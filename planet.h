#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <main.cpp>

#include <SFML/Graphics.hpp>

using namespace std;

class Planet {
    public: 
        int ind;
        double m, r;
        double gconstant;
        double scale;
        double AU;
        double timestep;
        pair<double, double> pos;
        pair<double, double> vel;
        pair<double, double> acc;

        vector<pair<double, double> > path;

        Planet (int num, double mass, double radius, pair<double, double> position, pair<double, double> velocity, pair<double, double> acceleration) {
            ind = num;
            m = mass;
            r = radius;
            gconstant = 6.67430e-11;
            AU = 149.6e6 * 1000;
            scale = 250 / AU; // 1 AU = 250 screen pixels.
            timestep = 3600 * 100; // 1 day.
            pos = position;
            vel = velocity;
            acc = acceleration;
        }

        pair<double, double> calculateForce (Planet other){
            // Obtain positions
            double ox = other.pos.first;
            double oy = other.pos.second;
            double sx = pos.first;
            double sy = pos.second;

            double dx = sx - ox;
            double dy = sy - oy;

            double dist = sqrt(dx * dx + dy * dy);

            double force = gconstant * m * other.m / (dist * dist);
            double theta = atan(dy/dx);
            double force_x = cos(theta) * force;
            double force_y = sin(theta) * force;

            return make_pair(force_x, force_y);
        }

        void update(Engine env){
            double net_fx = 0;
            double net_fy = 0;
            for (int i = 0; i < env.planets.size(); i++){
                if (env.planets[i].ind == ind) continue;
                pair<double, double> net_f = calculateForce(env.planets[j]);
                net_fx += net_f.first;
                net_fy += net_f.second;
            }

            vel.first += net_fx / m * timestep;
            vel.second += net_fy / m * timestep;

            pos.first += vel.first * timestep;
            pos.second += vel.second * timestep;
            
            // Add to the path so we can trace the path the planet takes.
            path.push_back(make_pair(pos.first, pos.second));
        }

        void display(Engine env){
            double x = pos.first * scale + env.H_WIDTH;
            double y = pos.second * scale + env.H_HEIGHT;

            sf::CircleShape::CircleShape circle(r, 50);
            circle.setPosition(x, y);
            env.window->draw(circle);
        }
};