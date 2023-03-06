#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace sf;

int screenWidth = 1280*2;
int screenHeight = 720*2;
static int planetCount = 400;



class Planet : public CircleShape
{
    public:
        float mass;
        float xVelocity;
        float yVelocity;

        Vector2f velocity;

        float speed;
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

        void updateVelocity(Planet solarSystem[], float deltaTime)
        {

            for(int i = 0; i < planetCount; i++)
            {
                if(*this == solarSystem[i])
                    continue;
                
                updateVelocityDueToPlanetCollision(solarSystem[i], deltaTime);
                updateVelocityDueToGravity(solarSystem[i], deltaTime);
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

            updateSpeed();
        }

        void updateSpeed()
        {
            speed = sqrt(xVelocity * xVelocity + yVelocity * yVelocity);
            this->speed = speed;
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
                    cout << "Planet1xVelocity is nan" << endl;
                    Planet1xVelocity = 0;
                }
                if(isnan(Planet1yVelocity))
                {
                    cout << "Planet1yVelocity is nan" << endl;
                    Planet1yVelocity = 0;
                }
                if(isnan(Planet2xVelocity))
                {
                    cout << "Planet2xVelocity is nan" << endl;
                    Planet2xVelocity = 0;
                }
                if(isnan(Planet2yVelocity))
                {
                    cout << "Planet2yVelocity is nan" << endl;
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

        //make a to string function
        string toString()
        {
            string output = "Planet: ";
            output += "x: " + to_string(getPosition().x) + " y: " + to_string(getPosition().y) + " xVelocity: " + to_string(xVelocity) + " yVelocity: " + to_string(yVelocity) + " speed: " + to_string(speed) + " mass: " + to_string(mass) + " radius: " + to_string(getRadius());
            return output;
        }

    };


int main()
{
    float speed = 0;
    RenderWindow window(VideoMode(screenWidth, screenHeight), "SIM");

    Planet solarSystem [] = {Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::Blue), 
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,-50, Color::Black),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,-50, Color::Green),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,50, Color::Yellow),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,-50, Color::Magenta),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,50, Color::Cyan),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),50,0, Color::Blue),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),-50,0, Color::White),
                             Planet(25,1000000,((screenWidth / 2)),(screenHeight / 2),0,50, Color::Red),

                             };


    // Planet solarSystem [] = {Planet(25,1000000,((screenWidth / 2)),((screenHeight / 2)+25),0,0, Color::Blue), 
    //                          Planet(25,1000000,((screenWidth / 2)+500),((screenHeight / 2)),-50,0, Color::Red),
    //                          };


    //for each planet in the solar system set random position
    for(int i = 0; i < planetCount; i++)
    {
        // solarSystem[i].setMass(5000000);
        //set starting velocity to 0
        solarSystem[i].setVelocity(0,0);

        solarSystem[i].setMass(1000);
        solarSystem[i].setRadius(5);
        solarSystem[i].setPosition(rand() % screenWidth, rand() % screenHeight);
    }

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

        cout << "FPS: " << fps << endl;

        window.clear();

        for(int i = 0; i < planetCount; i++)
        {
            solarSystem[i].updateVelocity(solarSystem, deletaTime);
            solarSystem[i].move(solarSystem[i].xVelocity * deletaTime, solarSystem[i].yVelocity * deletaTime);
        }

        for(int i = 0; i < planetCount; i++)
        {
            window.draw(solarSystem[i]);
        }

        // window.draw(solarSystem[0].velocityVector, 2, Lines);
        // window.draw(solarSystem[1].velocityVector, 2, Lines);

        window.display();
    }



    return 0;
}