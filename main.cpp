#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// g++ main.cpp -I/opt/homebrew/Cellar/sfml/2.5.1_2/include -o app -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib -lsfml-graphics -lsfml-window -lsfml-system

using namespace std;

class Planet {
    public: 
        string name;
        double m, r;
        double gconstant;
        double scale;
        double AU;
        double timestep;
        pair<double, double> pos;
        pair<double, double> vel;
        pair<double, double> net_f;

        vector<pair<double, double> > path;

        Planet (string s, double mass, double radius, pair<double, double> position, pair<double, double> velocity, pair<double, double> force) {
            name = s;
            m = mass;
            r = radius;
            gconstant = 6.67430e-11;
            AU = 149.6e6 * 1000;
            scale = 150 / AU; 
            timestep = 3600 * 100; // 1 day.
            pos = position;
            vel = velocity;
            net_f = force;
        }

        void calculateForce (vector<Planet> planets){
            net_f.first = 0;
            net_f.second = 0;

            for (int i = 0; i < planets.size(); i++){
                Planet other = planets[i];
                if (other.name == name) continue;
                // Obtain positions
                double ox = other.pos.first;
                double oy = other.pos.second;
                double sx = pos.first;
                double sy = pos.second;

                double dx = sx - ox;
                double dy = sy - oy;

                double dist = sqrt(dx * dx + dy * dy);

                double force = gconstant * m * other.m / (dist * dist);
                double force_x, force_y = 0;
                
                // Have to use the abs and xm, ym approach because otherwise this will break due to 
                // how the screen coordinate system works and how actual cartesian coordinates work.
                double theta = abs(atan(dy/dx));
                double xm = -1;
                double ym = -1;
                if (dx < 0) xm = 1;
                if (dy < 0) ym = 1;
                force_x = cos(theta) * force * xm;
                force_y = sin(theta) * force * ym;
                
                net_f.first += force_x;
                net_f.second += force_y;
            }
        }

        void update(){
            vel.first += net_f.first / m * timestep;
            vel.second += net_f.second / m * timestep;

            pos.first += vel.first * timestep;
            pos.second += vel.second * timestep;

            // Add to the path so we can trace the path the planet takes.
            path.push_back(make_pair(pos.first, pos.second));
        }
};

// Declaring the Engine class.
class Engine {
    public:
        int WIDTH, HEIGHT, H_WIDTH, H_HEIGHT;
        int FRAMERATE;

        // SFML Stuff:
        sf::RenderWindow* window;
        sf::VideoMode videoMode;
        
        // Engine stuff:
        vector<Planet> planets;
        int showTrail;

        Engine(int w, int h, int frameRate, bool trail){
            WIDTH = w;
            HEIGHT = h;
            H_WIDTH = WIDTH/2;
            H_HEIGHT = HEIGHT/2;
            FRAMERATE = frameRate;
            videoMode.height = HEIGHT;
            videoMode.width = WIDTH;
            window = new sf::RenderWindow(videoMode, "Planet Engine");
            window->setFramerateLimit(frameRate);
            planets;
            showTrail = trail;
        }

        void display(){
            for (int i = 0; i < planets.size(); i++){
                double x = planets[i].pos.first * planets[i].scale + H_WIDTH;
                double y = planets[i].pos.second * planets[i].scale + H_HEIGHT;

                sf::CircleShape circle(planets[i].r, 50);
                circle.setPosition(x, y);

                if (showTrail){
                    sf::CircleShape trail(1, 10);
                    for (int j = 0; j < planets[i].path.size(); j++){
                        trail.setFillColor(sf::Color(100, 250, 50));

                        trail.setPosition(
                            planets[i].path[j].first * planets[i].scale + H_WIDTH + planets[i].r/2, 
                            planets[i].path[j].second * planets[i].scale + H_HEIGHT + planets[i].r/2);

                        window->draw(trail);
                    }
                }

                if (planets[i].name == "Sun") circle.setFillColor(sf::Color(255, 100, 100));
                if (planets[i].name == "Earth") circle.setFillColor(sf::Color(0, 210, 255));
                if (planets[i].name == "Moon") circle.setFillColor(sf::Color(200, 200, 200));
                if (planets[i].name == "Commet") circle.setFillColor(sf::Color(100, 100, 255));
                if (planets[i].name == "LYX") circle.setFillColor(sf::Color(180, 100, 255));
                if (planets[i].name == "DSW") circle.setFillColor(sf::Color(100, 250, 205));
                
                window->draw(circle);
            }
        }
};

int main(){
    //! |Change the parameters to modify the engine|
    Engine engine1 = Engine(1920, 1080, 60, 1);
    //! |------------------------------------------|

    Planet sun = Planet("Sun", 1.98892e30, 30, make_pair(0, 0), make_pair(10, 0), make_pair(0, 0));
    Planet sun2 = Planet("Second Sun", 1.98892e30, 30, make_pair(-2 * sun.AU, 0), make_pair(0, 0), make_pair(0, 0));
    Planet earth = Planet("Earth", 5.9742e24, 16, make_pair(-1 * sun.AU, 0), make_pair(0, 29.783 * 1000), make_pair(0, 0));
    Planet mars = Planet("Mars", 6.39e23, 14, make_pair(-1.524 * sun.AU, 0), make_pair(0, 24.077 * 1000), make_pair(0, 0));
    Planet moon = Planet("Moon", 7.34767e22, 5, make_pair(-1.002569 * sun.AU, 0), make_pair(0, 1.022 * 1000 + 29.783 * 1000), make_pair(0, 0));
    Planet commet = Planet("Commet", 7.34767e25, 5, make_pair(-3 * sun.AU, 0), make_pair(3000, 6 * 1000), make_pair(0, 0));

    engine1.planets.push_back(sun);
    //engine1.planets.push_back(sun2);
    engine1.planets.push_back(earth);
    engine1.planets.push_back(mars);
    engine1.planets.push_back(moon);
    engine1.planets.push_back(commet);
    
    // Stable 3 body star system, exclude ily1 to get stable binary star system.:
    Planet lyx1 = Planet("LYX", 2e30, 30, make_pair(-1 * sun.AU, 0), make_pair(0, -15000), make_pair(0, 0));
    Planet dsw1 = Planet("DSW", 2e30, 30, make_pair(1 * sun.AU, 0), make_pair(0, 15000), make_pair(0, 0));
    Planet ily1 = Planet("ILY", 2e25, 25, make_pair(0, 0), make_pair(0, 0), make_pair(0, 0));
    // engine1.planets.push_back(lyx1);
    // engine1.planets.push_back(dsw1);  
    // engine1.planets.push_back(ily1);

    // Chaotic 3 Body problem:
    Planet lyx2 = Planet("LYX", 3e30, 30, make_pair(-1 * sun.AU, 0), make_pair(0, -20000), make_pair(0, 0));
    Planet dsw2 = Planet("DSW", 2e30, 30, make_pair(1*sun.AU, 0), make_pair(0, 20000), make_pair(0, 0));
    Planet ily2 = Planet("ILY", 2e30, 20, make_pair(0, 0), make_pair(2000, 1000), make_pair(0, 0));
    // engine1.planets.push_back(lyx2);
    // engine1.planets.push_back(dsw2);  
    // engine1.planets.push_back(ily2);


    double bx, by, cx, cy;
    double ex, ey;
    bool pressed = false;

    while (engine1.window->isOpen()){
        sf::Event event;
        while (engine1.window->pollEvent(event)){
            if (event.type == sf::Event::Closed) engine1.window->close();
            if (event.type == sf::Event::MouseMoved) {
                    cx = sf::Mouse::getPosition(*engine1.window).x;
                    cy = sf::Mouse::getPosition(*engine1.window).y;
                    
                    if (pressed){
                        double dx = cx - bx;
                        double dy = cy - by;
                        cout << "Dragging!" << endl;
                        sf::Vertex line[] = {
                            sf::Vertex(sf::Vector2f(bx, by)),
                            sf::Vertex(sf::Vector2f(cx, cy))
                        };
                        engine1.window->draw(line, 10, sf::Lines);
                    }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                cx = sf::Mouse::getPosition(*engine1.window).x;
                cy = sf::Mouse::getPosition(*engine1.window).y;

                if (!pressed){
                    cout << "Initial Press!" << endl;
                    pressed = true;
                    bx = cx;
                    by = cy;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                pressed = false;
                ex = sf::Mouse::getPosition(*engine1.window).x;
                ey = sf::Mouse::getPosition(*engine1.window).y;

                double dx = ex - bx;
                double dy = ex - by;


                Planet newplanet = Planet(
                    "Planet Object", 
                    2e28, 
                    25, 
                    make_pair( 
                        (ex - engine1.H_WIDTH) / sun.scale, 
                        (ey - engine1.H_HEIGHT) / sun.scale),
                    make_pair(0, 0), 
                    make_pair(0, 0));
                engine1.planets.push_back(newplanet);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
                cout << "what" << endl;
                engine1.showTrail = !engine1.showTrail;
            }
        }

        engine1.window->clear(sf::Color(0, 0, 0));

        for (int i = 0; i < engine1.planets.size(); i++) engine1.planets[i].calculateForce(engine1.planets);
        for (int i = 0; i < engine1.planets.size(); i++) engine1.planets[i].update();
        

        engine1.display();

        engine1.window->display();
    }


    cout << "Hello World!" << endl;
    return 0;
}



