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

int screenWidth = 1280;
int screenHeight = 720;

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

                //add planet to all overlapping squares as well dont include the original square or the squares that dont exist/ are out of bounds
                if(corispodingRowLeft != corispodingRow && corispodingRowLeft >= 0 && corispodingRowLeft < rows)
                {
                    int corispodingIndexLeft = corispodingRowLeft + corispodingCol * rows;
                    grid[corispodingIndexLeft].planets.push_back(&planets[i]);
                }
                if(corispodingColTop != corispodingCol && corispodingColTop >= 0 && corispodingColTop < columns)
                {
                    int corispodingIndexTop = corispodingRow + corispodingColTop * rows;
                    grid[corispodingIndexTop].planets.push_back(&planets[i]);
                }
                if(corispodingRowRight != corispodingRow && corispodingRowRight >= 0 && corispodingRowRight < rows)
                {
                    int corispodingIndexRight = corispodingRowRight + corispodingCol * rows;
                    grid[corispodingIndexRight].planets.push_back(&planets[i]);
                }
                if(corispodingColBottom != corispodingCol && corispodingColBottom >= 0 && corispodingColBottom < columns)
                {
                    int corispodingIndexBottom = corispodingRow + corispodingColBottom * rows;
                    grid[corispodingIndexBottom].planets.push_back(&planets[i]);
                }
                if(corispodingRowLeft != corispodingRow && corispodingRowLeft >= 0 && corispodingRowLeft < rows && corispodingColTop != corispodingCol && corispodingColTop >= 0 && corispodingColTop < columns)
                {
                    int corispodingIndexTopLeft = corispodingRowLeft + corispodingColTop * rows;
                    grid[corispodingIndexTopLeft].planets.push_back(&planets[i]);
                }
                if(corispodingRowRight != corispodingRow && corispodingRowRight >= 0 && corispodingRowRight < rows && corispodingColTop != corispodingCol && corispodingColTop >= 0 && corispodingColTop < columns)
                {
                    int corispodingIndexTopRight = corispodingRowRight + corispodingColTop * rows;
                    grid[corispodingIndexTopRight].planets.push_back(&planets[i]);
                }
                if(corispodingRowLeft != corispodingRow && corispodingRowLeft >= 0 && corispodingRowLeft < rows && corispodingColBottom != corispodingCol && corispodingColBottom >= 0 && corispodingColBottom < columns)
                {
                    int corispodingIndexBottomLeft = corispodingRowLeft + corispodingColBottom * rows;
                    grid[corispodingIndexBottomLeft].planets.push_back(&planets[i]);
                }
                if(corispodingRowRight != corispodingRow && corispodingRowRight >= 0 && corispodingRowRight < rows && corispodingColBottom != corispodingCol && corispodingColBottom >= 0 && corispodingColBottom < columns)
                {
                    int corispodingIndexBottomRight = corispodingRowRight + corispodingColBottom * rows;
                    grid[corispodingIndexBottomRight].planets.push_back(&planets[i]);
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
            updateForcesDueToGravity(deltaTime);
            updateForcesDueToWindowEdges();
        }

        void updateAllForcees(float deltaTime)
        {
            checkForPlanetColisions(); 
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


class QuadTree
{
public:
    QuadTree *northWest;
    QuadTree *northEast;
    QuadTree *southWest;
    QuadTree *southEast;

    vector <Planet*> points;

    int MaxPoints;
    bool divided;

    int x;
    int y;
    int width;
    int height;

    QuadTree()
    {
        northWest = nullptr;
        northEast = nullptr;
        southWest = nullptr;
        southEast = nullptr;
        MaxPoints = 3;
        divided = false;
    }

    QuadTree(int x, int y, int width, int height)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        northWest = nullptr;
        northEast = nullptr;
        southWest = nullptr;
        southEast = nullptr;
        MaxPoints = 3;
        divided = false;
    }

    void insert(Planet& point)
    {
        if(!divided)
        {
            if(points.size() < MaxPoints)
            {
                points.push_back(&point);
            }
            else
            {
                subdivide();
                insert(point);
            }
        }
        else
        {
            if(point.getPosition().x - point.getRadius() < x && point.getPosition().y - point.getRadius() < y)
            {
                northWest->insert(point);
            }
            if(point.getPosition().x + point.getRadius() > x && point.getPosition().y - point.getRadius() < y)
            {
                northEast->insert(point);
            }
            if(point.getPosition().x - point.getRadius() < x && point.getPosition().y + point.getRadius() > y)
            {
                southWest->insert(point);
            }
            if(point.getPosition().x + point.getRadius() > x && point.getPosition().y + point.getRadius() > y)
            {
                southEast->insert(point);
            }
        }
    }

    void subdivide()
    {
        northWest = new QuadTree(x - width/4, y - height/4, width/2, height/2);
        northEast = new QuadTree(x + width/4, y - height/4, width/2, height/2);
        southWest = new QuadTree(x - width/4, y + height/4, width/2, height/2);
        southEast = new QuadTree(x + width/4, y + height/4, width/2, height/2);
        divided = true;
        for(int i = 0; i < points.size(); i++)
        {
            insert(*points[i]);
        }
        points.clear();
    }

    float getDistanceBetweenPlanets(Planet &planet1, Planet &planet2)
    {
        float xDistance = planet1.getPosition().x - planet2.getPosition().x;
        float yDistance = planet1.getPosition().y - planet2.getPosition().y;
        return sqrt(xDistance * xDistance + yDistance * yDistance);
    }

    void draw(RenderWindow &window)
    {
        
        if(divided)
        {
            northWest->draw(window);
            northEast->draw(window);
            southWest->draw(window);
            southEast->draw(window);
        }
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f((x - width/2), (y - height/2))),
            sf::Vertex(sf::Vector2f((x + width/2) + 1, (y - height/2))),
            sf::Vertex(sf::Vector2f((x + width/2) + 1, (y + height/2) + 1)),
            sf::Vertex(sf::Vector2f((x - width/2), (y + height/2) + 1)),
            sf::Vertex(sf::Vector2f((x - width/2), (y + height/2) + 1)),
            sf::Vertex(sf::Vector2f((x - width/2), (y - height/2))),
        }; 
        window.draw(line, 6, sf::Lines);
    }

    void printEveryNodeValue()
    {
        cout << "Node: " << this->toString() << endl;
        cout << "Points: " << endl;
        for(int i = 0; i < points.size(); i++)
        {
            cout << points[i]->toString() << endl;
        }
        cout << endl;

        if(divided)
        {
            northWest->printEveryNodeValue();
            northEast->printEveryNodeValue();
            southWest->printEveryNodeValue();
            southEast->printEveryNodeValue();
        }
    }

    void WipeTree()
    {
        if(divided)
        {
            northWest->WipeTree();
            northEast->WipeTree();
            southWest->WipeTree();
            southEast->WipeTree();
            delete northWest;
            delete northEast;
            delete southWest;
            delete southEast;
        }
        points.clear();
        divided = false;
    }


    float angleBetweenRadians(Planet &planet1, Planet &planet2)
    {
        float x = planet1.getPosition().x - planet2.getPosition().x;
        float y = planet1.getPosition().y - planet2.getPosition().y;

        float angle = atan2(y, x);

        return angle;
    }
    
    void updateColorOfPlanetsInTree()
    {
        for(int i = 0; i < points.size(); i++)
        {
            points[i]->setFillColor(Color(255, 0, 0));
        }
        if(divided)
        {
            northWest->updateColorOfPlanetsInTree();
            northEast->updateColorOfPlanetsInTree();
            southWest->updateColorOfPlanetsInTree();
            southEast->updateColorOfPlanetsInTree();
        }
    }

    void checkForCollisions()
    {
        for(int i = 0; i < points.size(); i++)
        {
            for(int j = i + 1; j < points.size(); j++)
            {
                if(i != j)
                {
                    if(getDistanceBetweenPlanets(*points[i], *points[j]) < points[i]->getRadius() + points[j]->getRadius())
                    {
                        float distance = getDistanceBetweenPlanets(*points[i], *points[j]);
                        float overlap = points[i]->getRadius() + points[j]->getRadius() - distance;
                        float angle = angleBetweenRadians(*points[i], *points[j]);

                        points[i]->setPosition(points[i]->getPosition().x + overlap * cos(angle), points[i]->getPosition().y + overlap * sin(angle));

                        float Planet1xVelocity = (points[i]->getVelocity().x - ((2 * points[j]->getMass() / (points[i]->getMass() + points[j]->getMass())) * ((((points[i]->getVelocity().x - points[j]->getVelocity().x) * (points[i]->getPosition().x - points[j]->getPosition().x)) + ((points[i]->getVelocity().y - points[j]->getVelocity().y) * (points[i]->getPosition().y - points[j]->getPosition().y))) / (getDistanceBetweenPlanets(*points[i], *points[j]) * getDistanceBetweenPlanets(*points[i], *points[j]))) * (points[i]->getPosition().x - points[j]->getPosition().x)));
                        float Planet1yVelocity = (points[i]->getVelocity().y - ((2 * points[j]->getMass() / (points[i]->getMass() + points[j]->getMass())) * ((((points[i]->getVelocity().x - points[j]->getVelocity().x) * (points[i]->getPosition().x - points[j]->getPosition().x)) + ((points[i]->getVelocity().y - points[j]->getVelocity().y) * (points[i]->getPosition().y - points[j]->getPosition().y))) / (getDistanceBetweenPlanets(*points[i], *points[j]) * getDistanceBetweenPlanets(*points[i], *points[j]))) * (points[i]->getPosition().y - points[j]->getPosition().y)));

                        float Planet2xVelocity = (points[j]->getVelocity().x - ((2 * points[j]->getMass() / (points[i]->getMass() + points[j]->getMass())) * ((((points[j]->getVelocity().x - points[i]->getVelocity().x) * (points[j]->getPosition().x - points[i]->getPosition().x)) + ((points[j]->getVelocity().y - points[i]->getVelocity().y) * (points[j]->getPosition().y - points[i]->getPosition().y))) / (getDistanceBetweenPlanets(*points[i], *points[j]) * getDistanceBetweenPlanets(*points[i], *points[j]))) * (points[j]->getPosition().x - points[i]->getPosition().x)));
                        float Planet2yVelocity = (points[j]->getVelocity().y - ((2 * points[j]->getMass() / (points[i]->getMass() + points[j]->getMass())) * ((((points[j]->getVelocity().x - points[i]->getVelocity().x) * (points[j]->getPosition().x - points[i]->getPosition().x)) + ((points[j]->getVelocity().y - points[i]->getVelocity().y) * (points[j]->getPosition().y - points[i]->getPosition().y))) / (getDistanceBetweenPlanets(*points[i], *points[j]) * getDistanceBetweenPlanets(*points[i], *points[j]))) * (points[j]->getPosition().y - points[i]->getPosition().y)));

                        points[i]->setVelocity(Planet1xVelocity, Planet1yVelocity);
                        points[j]->setVelocity(Planet2xVelocity, Planet2yVelocity);
                    }
                }
            }
        }
        if(divided)
        {
            northWest->checkForCollisions();
            northEast->checkForCollisions();
            southWest->checkForCollisions();
            southEast->checkForCollisions();
        }
    }

    string toString()
    {
        return "(" + to_string(x) + ", " + to_string(y) + ")";
    }
};


int main()
{
    SolarSystem solarSystem(8);

    solarSystem.setRadiusOfallPlanets(10);
    solarSystem.randomizePositionOfPlnats();
    solarSystem.setPlanetsRandomColor();
    solarSystem.setMassOfAllPlanets(10000);
    UniformGrid UniformGrid(8,8);
    RenderWindow window(VideoMode(screenWidth, screenHeight), "SIM");

    QuadTree tree(screenWidth/2, screenHeight/2, screenWidth, screenHeight);

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

        

        tree.WipeTree();

        for(int i = 0; i < solarSystem.planets.size(); i++)
        {
            tree.insert(solarSystem.planets[i]);
        }

        tree.checkForCollisions();

        // solarSystem.updateForcesDueToGravity(deltaTime);
        
        solarSystem.updateForcesDueToWindowEdges();
        
        solarSystem.updatePlanetsPosition(deltaTime);

        window.clear();
        tree.draw(window);
        solarSystem.draw(window);
        window.display();
    }
    return 0;
}