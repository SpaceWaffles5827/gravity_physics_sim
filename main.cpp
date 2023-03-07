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

int screenWidth = 1280*2;
int screenHeight = 720*2;

class Planet : public CircleShape
{
    public:
        float mass;
        float xVelocity;
        float yVelocity;

        Vector2f velocity;

        Vertex velocityVector[2];

        Planet()
        {
            this->setRadius(10);
            mass = 0;
            xVelocity = 0;
            yVelocity = 0;
            velocity = Vector2f(0, 0);
        }

        Planet(float radious, float mass, float xPos, float yPos, float xVelocity, float yVelocity, Color color)
        {
            this->setPosition(xPos, yPos);
            this->setRadius(radious);
            this->mass = mass;
            this->xVelocity = xVelocity;
            this->yVelocity = yVelocity;
            this->setFillColor(color);
            this->setOrigin(getRadius(), getRadius());
            this->setFillColor(color);
            velocity = Vector2f(xVelocity, yVelocity);
        }

        void setVelocity(float xVelocity, float yVelocity)
        {
            this->xVelocity = xVelocity;
            this->yVelocity = yVelocity;
            velocity = Vector2f(xVelocity, yVelocity);
        }

        void setRadius(float radius)
        {
            this->CircleShape::setRadius(radius);
            this->setOrigin(getRadius(), getRadius());
        }

        void updateVelocity(Planet planets[], int numberOfPlanets ,float deltaTime)
        {
            for(int i = 0; i < numberOfPlanets; i++)
            {
                if(*this == planets[i])
                    continue;
                
                updateVelocityDueToPlanetCollision(planets[i], deltaTime);
                updateVelocityDueToGravity(planets[i], deltaTime);
            }

            updateVelocityDueToWindowBounds();

            updateVelocityVector();
        }

        bool operator==(Planet &a)
        {
            if(this == &a)
                return true;
            else
                return false;
        }

        void updateVelocityVector()
        {
            velocityVector[0].position = getPosition();
            velocityVector[1].position = Vector2f(getPosition().x + xVelocity, getPosition().y + yVelocity);
            velocityVector[0].color = Color::Red;
            velocityVector[1].color = Color::Red;
        }

        void setMass(float mass)
        {
            this->mass = mass;
        }

    private:

        void testSolarSystemClass()
        {
            cout << "test123" << endl;
        }


        void setRandonPosition()
        {
            this->setPosition(rand() % screenWidth, rand() % screenHeight);
        }

        float getCurentSpeed()
        {
            return sqrt(xVelocity * xVelocity + yVelocity * yVelocity);
        }

        void updateVelocityDueToGravity(Planet &planet, float deltaTime)
        {
            
            float acceleration = accelerationBetween(*this, planet);
            float angle = angleBetweenRadians(*this, planet);

            xVelocity -= acceleration * cos(angle) * deltaTime;
            yVelocity -= acceleration * sin(angle) * deltaTime;

        }

        bool updateVelocityDueToPlanetCollision(Planet &planet, float deltaTime)
        {
            if(distanceBetween(*this, planet) >= this->getRadius() + planet.getRadius())
            {
                return false;
            }
            else
            {
                if(distanceBetween(*this, planet) < this->getRadius() + planet.getRadius())
                {
                    float distance = distanceBetween(*this, planet);
                    float overlap = this->getRadius() + planet.getRadius() - distance;
                    float angle = angleBetweenRadians(*this, planet);

                    this->setPosition(getPosition().x + overlap * cos(angle), getPosition().y + overlap * sin(angle));
                }

                float Planet1xVelocity = (xVelocity - ((2 * planet.mass / (mass + planet.mass)) * ((((xVelocity - planet.xVelocity) * (getPosition().x - planet.getPosition().x)) + ((yVelocity - planet.yVelocity) * (getPosition().y - planet.getPosition().y))) / (distanceBetween(*this,planet) * distanceBetween(*this,planet))) * (getPosition().x - planet.getPosition().x)));
                float Planet1yVelocity = (yVelocity - ((2 * planet.mass / (mass + planet.mass)) * ((((xVelocity - planet.xVelocity) * (getPosition().x - planet.getPosition().x)) + ((yVelocity - planet.yVelocity) * (getPosition().y - planet.getPosition().y))) / (distanceBetween(*this,planet) * distanceBetween(*this,planet))) * (getPosition().y - planet.getPosition().y)));

                float Planet2xVelocity = (planet.xVelocity - ((2 * mass / (mass + planet.mass)) * ((((planet.xVelocity - xVelocity) * (planet.getPosition().x - getPosition().x)) + ((planet.yVelocity - yVelocity) * (planet.getPosition().y - getPosition().y))) / (distanceBetween(*this,planet) * distanceBetween(*this,planet))) * (planet.getPosition().x - getPosition().x)));
                float Planet2yVelocity = (planet.yVelocity - ((2 * mass / (mass + planet.mass)) * ((((planet.xVelocity - xVelocity) * (planet.getPosition().x - getPosition().x)) + ((planet.yVelocity - yVelocity) * (planet.getPosition().y - getPosition().y))) / (distanceBetween(*this,planet) * distanceBetween(*this,planet))) * (planet.getPosition().y - getPosition().y)));

                if(isnan(Planet1xVelocity))
                {
                    cout << "Error Planet1xVelocity is nan" << endl;
                    Planet1xVelocity = 0;
                }
                if(isnan(Planet1yVelocity))
                {
                    cout << "Error Planet1yVelocity is nan" << endl;
                    Planet1yVelocity = 0;
                }
                if(isnan(Planet2xVelocity))
                {
                    cout << "Error Planet2xVelocity is nan" << endl;
                    Planet2xVelocity = 0;
                }
                if(isnan(Planet2yVelocity))
                {
                    cout << "Error Planet2yVelocity is nan" << endl;
                    Planet2yVelocity = 0;
                }

                xVelocity = Planet1xVelocity;
                yVelocity = Planet1yVelocity;

                planet.xVelocity = Planet2xVelocity;
                planet.yVelocity = Planet2yVelocity;

                return true;
            }
            return false;
        }

        bool updateVelocityDueToWindowBounds()
        {
            if(getPosition().x + getRadius() > screenWidth)
            {
                this->setPosition(screenWidth - getRadius(), getPosition().y);
                xVelocity = -xVelocity;
                return true;
            }
            if(getPosition().x - getRadius() < 0)
            {
                this->setPosition(getRadius(), getPosition().y);
                xVelocity = -xVelocity;
                return true;
            }
            if(getPosition().y + getRadius() > screenHeight)
            {
                this->setPosition(getPosition().x, screenHeight - getRadius());
                yVelocity = -yVelocity;
                return true;
            }
            if(getPosition().y - getRadius() < 0)
            {
                this->setPosition(getPosition().x, getRadius());
                yVelocity = -yVelocity;
                return true;
            }
            return false;
        }

        float distanceBetween(Planet &a, Planet &b)
        {
            float x = a.getPosition().x - b.getPosition().x;
            float y = a.getPosition().y - b.getPosition().y;

            return sqrt(x * x + y * y);
        }

        float forceBetween(Planet &a, Planet &b)
        {
            float distance = distanceBetween(a, b);
            float force = (a.mass * b.mass) / (distance * distance);

            return force;
        }

        float accelerationBetween(Planet &a, Planet &b)
        {
            if(distanceBetween(a, b) == 0)
                return 0;
            float force = forceBetween(a, b);
            float acceleration = force / a.mass;

            return acceleration;
        }

        float angleBetween(Planet &a, Planet &b)
        {
            float x = a.getPosition().x - b.getPosition().x;
            float y = a.getPosition().y - b.getPosition().y;

            float angle = atan2(y, x) * 180 / 3.14159265;

            return angle;
        }

        float angleBetweenRadians(Planet &a, Planet &b)
        {
            float x = a.getPosition().x - b.getPosition().x;
            float y = a.getPosition().y - b.getPosition().y;

            float angle = atan2(y, x);

            return angle;
        }

        string toString()
        {
            string output = "Planet: ";
            output += "x: " + to_string(getPosition().x) + " y: " + to_string(getPosition().y) + " xVelocity: " + to_string(xVelocity) + " yVelocity: " + to_string(yVelocity) + " speed: " + to_string(getCurentSpeed()) + " mass: " + to_string(mass) + " radius: " + to_string(getRadius());
            return output;
        }

};


class solarSystem
{
    private:
        int numberOfPlanets;
        Planet* planets; 

    public:

        solarSystem()
        {
            numberOfPlanets = 0;
            planets = new Planet[numberOfPlanets];
        }

        solarSystem(int numberOfPlanets)
        { 
            this->numberOfPlanets = numberOfPlanets;
            planets = new Planet[numberOfPlanets];
        } 

        void updateVelocityAndMove(float deletaTime)
        {
             for(int i = 0; i < numberOfPlanets; i++)
            {
                this->planets[i].updateVelocity(planets, numberOfPlanets , deletaTime);
                this->planets[i].move(this->planets[i].xVelocity * deletaTime, this->planets[i].yVelocity * deletaTime);
            }
        }

        void drawEveryPlanet(RenderWindow &window)
        {
            for(int i = 0; i < numberOfPlanets; i++)
            {
                window.draw(planets[i]);
            }
        }

        void Init()
        {
            for (int i = 0; i < numberOfPlanets; i++)
            {
                planets[i] = Planet(25, 100, rand() % screenWidth, rand() % screenHeight, 0, 0, Color::White);
            }
        }

        void setMassOfAllPlanets(int mass)
        {
            for (int i = 0; i < numberOfPlanets; i++)
            {
                planets[i].mass = mass;
            }
        }

        void RandomizeLocations()
        {
            for (int i = 0; i < numberOfPlanets; i++)
            {
                planets[i] = Planet(25, 100, rand() % screenWidth, rand() % screenHeight, 0, 0, Color::White);
            }
        }

        void RandomizeColors()
        {
            for (int i = 0; i < numberOfPlanets; i++)
            {
                planets[i].setFillColor(Color(rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55));
            }
        }

        void setColor(int i,Color color)
        {
            planets[i].setFillColor(color);
        }

        void setRadiousOfAllPlanets(int radius)
        {
            for (int i = 0; i < numberOfPlanets; i++)
            {
                planets[i].setRadius(radius);
            }
        }

        void RandomizeSizes(int min, int max)
        {
            for (int i = 0; i < numberOfPlanets; i++)
            {
                planets[i].setRadius(rand() % max + min);
            }
        }

        int getNumberOfPlanets()
        {
            return numberOfPlanets;
        }

        Planet getPlanet(int index)
        {
            return planets[index];
        }

        Planet* getPlanets()
        {
            return planets;
        }
};

class singleGrid
{
    public:
        int row;
        int col;
        vector<Planet> planets;

        string toString()
        {
            string output = "Grid: ";
            output += "row: " + to_string(row) + " totalCol: " + to_string(col);
            return output;
        }
};

class uniformGrid
{
    public:
        int totalRow;
        int totalCol;
        vector<singleGrid> grids;
        
        uniformGrid()
        {
            this->totalRow = 2;
            this->totalCol = 2;
        }

        uniformGrid(int totalRow, int totalCol)
        {
            this->totalRow = totalRow;
            this->totalCol = totalCol;
        }

        void fillWithPlanets(solarSystem solarSystem)
        {
            for(int i = 0; solarSystem.getNumberOfPlanets() > i; i++)
            {
                // cout << solarSystem.getPlanet(i).getPosition().x << endl;

                int corispodingRow = solarSystem.getPlanet(i).getPosition().x / (screenWidth/totalRow);

                if(corispodingRow == 0)
                {
                    solarSystem.setColor(i,Color(255,0,0));
                }
                if(corispodingRow == 1)
                {
                    solarSystem.setColor(i,Color(0,255,0));
                }
                if(corispodingRow == 2)
                {
                    solarSystem.setColor(i,Color(0,0,255));
                }

                cout << corispodingRow << endl;


                // cout << solarSystem.getPlanet(i).getPosition().y << endl;


            }
        }

        string toString()
        {
            string output = "Uniform Grid: ";
            output += "totalRow: " + to_string(totalRow) + " totalCol: " + to_string(totalCol);
            return output;
        }
};


int main()
{
    uniformGrid uniformGrid(3,3);

    solarSystem solarSystem(200);
    solarSystem.RandomizeLocations();
    solarSystem.RandomizeColors();
    solarSystem.setRadiousOfAllPlanets(15);
    solarSystem.setMassOfAllPlanets(10000);
    // solarSystem.RandomizeSizes(5, 25);

    vector<int> list_name;

    list_name.push_back(1);
    list_name.push_back(2);


    cout << list_name.size() << endl;



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

        float deletaTime = clock.restart().asSeconds();
        float fps = 1 / deletaTime;

        window.setTitle("SIM - FPS: " + to_string(fps));

        uniformGrid.fillWithPlanets(solarSystem);

        solarSystem.updateVelocityAndMove(deletaTime);
        window.clear();
        solarSystem.drawEveryPlanet(window);

        // window.draw(solarSystem.getPlanet(0).velocityVector, 2, Lines);

        window.display();
    }
    return 0;
}