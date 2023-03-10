#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <iterator>
#include <list>

// No Grid FPS
//500 size 10 ~ 60 FPS
//1000 size 20 ~ 16 FPS

using namespace std;
using namespace sf;

int screenWidth = 2560;
int screenHeight = 1440;

struct VelocityVector
{
    float x;
    float y;
};

class Planet : public CircleShape
{
    private:
        VelocityVector velocityVector;
        float mass;
    
    public:
        Planet()
        {
            mass = 1000;
            velocityVector.x = 0;
            velocityVector.y = 0;
            setRadius(25);
            setPosition(0, 0);
            setFillColor(Color::White);
            setOrigin(25, 25);
        }

        Planet(float posX, float PosY)
        {
            mass = 1000;
            velocityVector.x = 0;
            velocityVector.y = 0;
            setRadius(25);
            setPosition(posX, PosY);
            setFillColor(Color::White);
            setOrigin(25, 25);
        }

        Planet(float posX, float PosY , float mass, float radius, float xVelocity, float yVelocity)
        {
            this->mass = mass;
            velocityVector.x = xVelocity;
            velocityVector.y = yVelocity;
            setRadius(radius);
            setPosition(posX, PosY);
            setFillColor(Color::White);
            setOrigin(radius, radius);
        }


        VelocityVector getVelocity()
        {
            return velocityVector;
        }

        float getMass()
        {
            return mass;
        }

        void setVelocity(float x, float y)
        {
            velocityVector.x = x;
            velocityVector.y = y;
        }

        void setMass(float mass)
        {
            this->mass = mass;
        }

        string toString()
        {
            return "Position: " + to_string(getPosition().x) + ", " + to_string(getPosition().y) + " Velocity: " + to_string(velocityVector.x) + ", " + to_string(velocityVector.y) + " Mass: " + to_string(mass);
        }
};

class GridSquare
{
    private:

        float getDistanceBetweenPlanets(Planet &planet1, Planet &planet2)
        {
            float xDistance = planet1.getPosition().x - planet2.getPosition().x;
            float yDistance = planet1.getPosition().y - planet2.getPosition().y;
            return sqrt(xDistance * xDistance + yDistance * yDistance);
        }

        float angleBetweenRadians(Planet &planet1, Planet &planet2)
        {
            float x = planet1.getPosition().x - planet2.getPosition().x;
            float y = planet1.getPosition().y - planet2.getPosition().y;

            float angle = atan2(y, x);

            return angle;
        }

    public:
        vector<Planet*> planets;
        GridSquare()
        {
            planets.resize(0);
        }

        void setColorOfPlanets(Color color)
        {
            for(int i = 0; i < planets.size(); i++)
            {
                planets[i]->setFillColor(color);
            }
        }

       

        void checkForCollisions()
        {
            for(int i = 0; i < planets.size(); i++)
            {
                for(int j = 0; j < planets.size(); j++)
                {
                    if(i != j)
                    {
                        if(getDistanceBetweenPlanets(*planets[i], *planets[j]) < planets[i]->getRadius() + planets[j]->getRadius())
                        {
                            float distance = getDistanceBetweenPlanets(*planets[i], *planets[j]);
                            float overlap = planets[i]->getRadius() + planets[j]->getRadius() - distance;
                            float angle = angleBetweenRadians(*planets[i], *planets[j]);

                            planets[i]->setPosition(planets[i]->getPosition().x + overlap * cos(angle), planets[i]->getPosition().y + overlap * sin(angle));

                            float Planet1xVelocity = (planets[i]->getVelocity().x - ((2 * planets[j]->getMass() / (planets[i]->getMass() + planets[j]->getMass())) * ((((planets[i]->getVelocity().x - planets[j]->getVelocity().x) * (planets[i]->getPosition().x - planets[j]->getPosition().x)) + ((planets[i]->getVelocity().y - planets[j]->getVelocity().y) * (planets[i]->getPosition().y - planets[j]->getPosition().y))) / (getDistanceBetweenPlanets(*planets[i], *planets[j]) * getDistanceBetweenPlanets(*planets[i], *planets[j]))) * (planets[i]->getPosition().x - planets[j]->getPosition().x)));
                            float Planet1yVelocity = (planets[i]->getVelocity().y - ((2 * planets[j]->getMass() / (planets[i]->getMass() + planets[j]->getMass())) * ((((planets[i]->getVelocity().x - planets[j]->getVelocity().x) * (planets[i]->getPosition().x - planets[j]->getPosition().x)) + ((planets[i]->getVelocity().y - planets[j]->getVelocity().y) * (planets[i]->getPosition().y - planets[j]->getPosition().y))) / (getDistanceBetweenPlanets(*planets[i], *planets[j]) * getDistanceBetweenPlanets(*planets[i], *planets[j]))) * (planets[i]->getPosition().y - planets[j]->getPosition().y)));

                            float Planet2xVelocity = (planets[j]->getVelocity().x - ((2 * planets[j]->getMass() / (planets[i]->getMass() + planets[j]->getMass())) * ((((planets[j]->getVelocity().x - planets[i]->getVelocity().x) * (planets[j]->getPosition().x - planets[i]->getPosition().x)) + ((planets[j]->getVelocity().y - planets[i]->getVelocity().y) * (planets[j]->getPosition().y - planets[i]->getPosition().y))) / (getDistanceBetweenPlanets(*planets[i], *planets[j]) * getDistanceBetweenPlanets(*planets[i], *planets[j]))) * (planets[j]->getPosition().x - planets[i]->getPosition().x)));
                            float Planet2yVelocity = (planets[j]->getVelocity().y - ((2 * planets[j]->getMass() / (planets[i]->getMass() + planets[j]->getMass())) * ((((planets[j]->getVelocity().x - planets[i]->getVelocity().x) * (planets[j]->getPosition().x - planets[i]->getPosition().x)) + ((planets[j]->getVelocity().y - planets[i]->getVelocity().y) * (planets[j]->getPosition().y - planets[i]->getPosition().y))) / (getDistanceBetweenPlanets(*planets[i], *planets[j]) * getDistanceBetweenPlanets(*planets[i], *planets[j]))) * (planets[j]->getPosition().y - planets[i]->getPosition().y)));

                            planets[i]->setVelocity(Planet1xVelocity, Planet1yVelocity);
                            planets[j]->setVelocity(Planet2xVelocity, Planet2yVelocity);
                        }
                    }
                }
            }
        }
};

class UniformGrid
{
    private:
        int rows;
        int columns;

    public:
        vector<GridSquare> grid;

        UniformGrid()
        {
            this->rows = 2;
            this->columns = 2;
            grid.resize(rows * columns);
        }

        UniformGrid(int rows, int columns)
        {
            this->rows = rows;
            this->columns = columns;
            grid.resize(rows * columns);
        }

        void draw(RenderWindow &window)
        {   
            //draw vertical lines
            for(int i = 1; i <= rows; i++)
            {
                Vertex line[] =
                {
                    Vertex(Vector2f((screenWidth/rows) * i, 0)),
                    Vertex(Vector2f((screenWidth/rows) * i, screenHeight)),
                };
                window.draw(line, 2, Lines);
            }
            //draw horizontal lines
            for(int i = 1; i <= columns; i++)
            {
                Vertex line[] =
                {
                    Vertex(Vector2f(0, (screenHeight/columns) * i)),
                    Vertex(Vector2f(screenWidth, (screenHeight/columns) * i)),
                };
                window.draw(line, 2, Lines);
            }
        }

        void checkForColisionsInEachGridSquare()
        {
            cout << this->toString() << endl;
            for(int i = 0; i < grid.size(); i++)
            {
                grid[i].checkForCollisions();
            }
        }

        void addPlanetsToGrid(vector<Planet> &planets)
        {
            for(int i = 0; i < grid.size(); i++)
            {
                // grid[i].planets.resize(0); original
                grid[i].planets.assign(grid[i].planets.size(), nullptr);
            }

            for(int i = 0; planets.size() > i; i++)
            {
                int corispodingRow = planets[i].getPosition().x / (screenWidth/rows);
                int corispodingCol = planets[i].getPosition().y / (screenHeight/columns);

                int corispodingRowLeft = (planets[i].getPosition().x - planets[i].getRadius()) / (screenWidth/rows);
                int corispodingColTop = (planets[i].getPosition().y - planets[i].getRadius()) / (screenHeight/columns);

                int corispodingRowRight = (planets[i].getPosition().x + planets[i].getRadius()) / (screenWidth/rows);
                int corispodingColBottom = (planets[i].getPosition().y + planets[i].getRadius()) / (screenHeight/columns);

                int corispodingIndex = corispodingRow + corispodingCol * rows;

                grid[corispodingIndex].planets.push_back(&planets[i]);

                //add planet to all overlapping squares as well

                if(corispodingRowLeft != corispodingRow)
                {
                    grid[corispodingRowLeft + corispodingCol * rows].planets.push_back(&planets[i]);
                }
                if(corispodingColTop != corispodingCol)
                {
                    grid[corispodingRow + corispodingColTop * rows].planets.push_back(&planets[i]);
                }
                if(corispodingRowRight != corispodingRow)
                {
                    grid[corispodingRowRight + corispodingCol * rows].planets.push_back(&planets[i]);
                }
                if(corispodingColBottom != corispodingCol)
                {
                    grid[corispodingRow + corispodingColBottom * rows].planets.push_back(&planets[i]);
                }
                if(corispodingRowLeft != corispodingRow && corispodingColTop != corispodingCol)
                {
                    grid[corispodingRowLeft + corispodingColTop * rows].planets.push_back(&planets[i]);
                }
                if(corispodingRowRight != corispodingRow && corispodingColTop != corispodingCol)
                {
                    grid[corispodingRowRight + corispodingColTop * rows].planets.push_back(&planets[i]);
                }
                if(corispodingRowLeft != corispodingRow && corispodingColBottom != corispodingCol)
                {
                    grid[corispodingRowLeft + corispodingColBottom * rows].planets.push_back(&planets[i]);
                }
                if(corispodingRowRight != corispodingRow && corispodingColBottom != corispodingCol)
                {
                    grid[corispodingRowRight + corispodingColBottom * rows].planets.push_back(&planets[i]);
                }


            }
        }

        string toString()
        {
            string output = "";
            for(int i = 0; i < grid.size(); i++)
            {
                if(i % rows == 0)
                {
                    output += "\n";
                }
                output += to_string(grid[i].planets.size()) + " ";
            }
            return output;
        }
};

class SolarSystem
{
    public:
        int numberOfPlanets;

    public:
        vector<Planet> planets;

        SolarSystem()
        {
            numberOfPlanets = 2;
            planets.resize(0);
            for(int i = 0; i < numberOfPlanets; i++)
            {
                planets.push_back(Planet());
            }
        }

        SolarSystem(int numberOfPlanets)
        {
            this->numberOfPlanets = numberOfPlanets;
            planets.resize(0);
            for(int i = 0; i < numberOfPlanets; i++)
            {
                planets.push_back(Planet());
            }
        }

        void setRadiusOfallPlanets(float radius)
        {
            for(int i = 0; i < numberOfPlanets; i++)
            {
                planets[i].setRadius(radius);
                planets[i].setOrigin(radius, radius);
            }
        }

        void setMassOfAllPlanets(float mass)
        {
            for(int i = 0; i < numberOfPlanets; i++)
            {
                planets[i].setMass(mass);
            }
        }

        void addPlanet(Planet planet)
        {
            planets.push_back(planet);
            numberOfPlanets++;
        }

        void updateForcesDueToWindowEdges()
        {
            for(int i = 0; i < numberOfPlanets; i++)
            {
                if(planets[i].getPosition().x + planets[i].getRadius() > screenWidth)
                {
                    planets[i].setPosition(screenWidth - planets[i].getRadius(), planets[i].getPosition().y);
                    planets[i].setVelocity(-planets[i].getVelocity().x, planets[i].getVelocity().y);
                }
                if(planets[i].getPosition().x - planets[i].getRadius() < 0)
                {
                    planets[i].setPosition(planets[i].getRadius(), planets[i].getPosition().y);
                    planets[i].setVelocity(-planets[i].getVelocity().x, planets[i].getVelocity().y);
                }
                if(planets[i].getPosition().y + planets[i].getRadius() > screenHeight)
                {
                    planets[i].setPosition(planets[i].getPosition().x, screenHeight - planets[i].getRadius());
                    planets[i].setVelocity(planets[i].getVelocity().x, -planets[i].getVelocity().y);
                }
                if(planets[i].getPosition().y - planets[i].getRadius() < 0)
                {
                    planets[i].setPosition(planets[i].getPosition().x, planets[i].getRadius());
                    planets[i].setVelocity(planets[i].getVelocity().x, -planets[i].getVelocity().y);
                }
            }
        }

        void updateAllForcees(float deltaTime,UniformGrid UniformGrid)
        {
            checkForPlanetColisions(UniformGrid);
            // checkForPlanetColisions(); 
            updateForcesDueToGravity(deltaTime);
            updateForcesDueToWindowEdges();
        }

        void checkForPlanetColisions()
        {            
            for(int i = 0; i < planets.size(); i++)
            {
                for(int j = i + 1; j < planets.size(); j++)
                {
                    if(distanceBetween(planets[i], planets[j]) < planets[i].getRadius() + planets[j].getRadius())
                    {
                            float distance = distanceBetween(planets[i], planets[j]);
                            float overlap = planets[i].getRadius() + planets[j].getRadius() - distance;
                            float angle = angleBetweenRadians(planets[i], planets[j]);

                            planets[i].setPosition(planets[i].getPosition().x + overlap * cos(angle), planets[i].getPosition().y + overlap * sin(angle));
                            

                            float Planet1xVelocity = (planets[i].getVelocity().x - ((2 * planets[j].getMass() / (planets[i].getMass() + planets[j].getMass())) * ((((planets[i].getVelocity().x - planets[j].getVelocity().x) * (planets[i].getPosition().x - planets[j].getPosition().x)) + ((planets[i].getVelocity().y - planets[j].getVelocity().y) * (planets[i].getPosition().y - planets[j].getPosition().y))) / (distanceBetween(planets[i], planets[j]) * distanceBetween(planets[i], planets[j]))) * (planets[i].getPosition().x - planets[j].getPosition().x)));
                            float Planet1yVelocity = (planets[i].getVelocity().y - ((2 * planets[j].getMass() / (planets[i].getMass() + planets[j].getMass())) * ((((planets[i].getVelocity().x - planets[j].getVelocity().x) * (planets[i].getPosition().x - planets[j].getPosition().x)) + ((planets[i].getVelocity().y - planets[j].getVelocity().y) * (planets[i].getPosition().y - planets[j].getPosition().y))) / (distanceBetween(planets[i], planets[j]) * distanceBetween(planets[i], planets[j]))) * (planets[i].getPosition().y - planets[j].getPosition().y)));

                            float Planet2xVelocity = (planets[j].getVelocity().x - ((2 * planets[j].getMass() / (planets[i].getMass() + planets[j].getMass())) * ((((planets[j].getVelocity().x - planets[i].getVelocity().x) * (planets[j].getPosition().x - planets[i].getPosition().x)) + ((planets[j].getVelocity().y - planets[i].getVelocity().y) * (planets[j].getPosition().y - planets[i].getPosition().y))) / (distanceBetween(planets[i], planets[j]) * distanceBetween(planets[i], planets[j]))) * (planets[j].getPosition().x - planets[i].getPosition().x)));
                            float Planet2yVelocity = (planets[j].getVelocity().y - ((2 * planets[j].getMass() / (planets[i].getMass() + planets[j].getMass())) * ((((planets[j].getVelocity().x - planets[i].getVelocity().x) * (planets[j].getPosition().x - planets[i].getPosition().x)) + ((planets[j].getVelocity().y - planets[i].getVelocity().y) * (planets[j].getPosition().y - planets[i].getPosition().y))) / (distanceBetween(planets[i], planets[j]) * distanceBetween(planets[i], planets[j]))) * (planets[j].getPosition().y - planets[i].getPosition().y)));

                            planets[i].setVelocity(Planet1xVelocity, Planet1yVelocity);
                            planets[j].setVelocity(Planet2xVelocity, Planet2yVelocity);
                    }
                }
            }
        }

        void checkForPlanetColisions(UniformGrid UniformGrid)
        {
            UniformGrid.addPlanetsToGrid(planets);
            UniformGrid.checkForColisionsInEachGridSquare();
        }

        void updatePlanetsPosition(float deltaTime)
        {
            for(int i = 0; i < numberOfPlanets; i++)
            {
                planets[i].move(planets[i].getVelocity().x * deltaTime, planets[i].getVelocity().y * deltaTime);
            }
        }

        void updateForcesDueToGravity(float deltaTime)
        {
            for(int i = 0; i < numberOfPlanets; i++)
            {
                for(int j = 0; j < numberOfPlanets; j++)
                {
                    if(i != j)
                    {
                        float acceleration = accelerationBetween(planets[i], planets[j]);
                        float angle = angleBetweenRadians(planets[i], planets[j]);

                        planets[i].setVelocity(planets[i].getVelocity().x - acceleration * cos(angle) * deltaTime, planets[i].getVelocity().y - acceleration * sin(angle) * deltaTime);
                    }
                }
            }
        }

        float accelerationBetween(Planet &planet1, Planet &planet2)
        {
            if(distanceBetween(planet1, planet2) == 0)
                return 0;
            float force = forceBetween(planet1, planet2);
            float acceleration = force / planet1.getMass();

            return acceleration;
        }

        float forceBetween(Planet &planet1, Planet &planet2)
        {
            float distance = distanceBetween(planet1, planet2);
            float force = (planet1.getMass() * planet2.getMass()) / (distance * distance);

            return force;
        }

        float distanceBetween(Planet &planet1, Planet &planet2)
        {
            float xDistance = planet1.getPosition().x - planet2.getPosition().x;
            float yDistance = planet1.getPosition().y - planet2.getPosition().y;
            return sqrt(xDistance * xDistance + yDistance * yDistance);
        }

        float angleBetweenRadians(Planet &planet1, Planet &planet2)
        {
            float x = planet1.getPosition().x - planet2.getPosition().x;
            float y = planet1.getPosition().y - planet2.getPosition().y;

            float angle = atan2(y, x);

            return angle;
        }

        void draw(RenderWindow &window)
        {
            for(int i = 0; i < numberOfPlanets; i++)
            {
                window.draw(planets[i]);
            }
        }

        void randomizePositionOfPlnats()
        {
            for(int i = 0; i < numberOfPlanets; i++)
            {
                planets[i].setPosition(rand() % screenWidth, rand() % screenHeight);
            }
        }

        void setPlanetsRandomColor()
        {
            for(int i = 0; i < numberOfPlanets; i++)
            {
                planets[i].setFillColor(Color(rand() % 255 + 50, rand() % 255 + 50, rand() % 255 + 50));
            }
        }

        void setPlanetColorWithIndex(int i)
        {
            planets[i].setFillColor(Color(255, 0, 0));
        }
};



int main()
{
    SolarSystem solarSystem(10);

    solarSystem.setRadiusOfallPlanets(25);
    solarSystem.randomizePositionOfPlnats();
    solarSystem.setPlanetsRandomColor();
    solarSystem.setMassOfAllPlanets(1000000);
    UniformGrid UniformGrid(3,3);
    RenderWindow window(VideoMode(screenWidth, screenHeight), "SIM");

    Clock clock;

    while (window.isOpen())
    {

        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::Resized)
            {
                FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(View(visibleArea));
            }
        }

        float deltaTime = clock.restart().asSeconds();
        float fps = 1 / deltaTime;

        window.setTitle("SIM - FPS: " + to_string(fps));

        
        solarSystem.updateAllForcees(deltaTime, UniformGrid);
        solarSystem.updatePlanetsPosition(deltaTime);

        // cout << "planet 0: " << solarSystem.planets[0].toString() << endl;        

        window.clear();
        UniformGrid.draw(window);
        solarSystem.draw(window);
        window.display();
    }
    return 0;
}