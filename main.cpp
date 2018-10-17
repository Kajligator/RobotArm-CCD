#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>
#include "segment.hpp"
#include "helpers.h"
#include <Eigen/Dense>

#define NUM_SEG 3
#define MAX_IK_TRIES 100
#define IK_POS_THRESH 1.0f
#define HEIGHT 1200
#define WIDTH 1200
#define DAMPING true
#define DOF_RESTRICTED true

float len = 100;
segment segments[] = {};
float x, y;
bool mouse_clicked = false;

void updateCursorPosition(sf::RenderWindow *window){
    x = sf::Mouse::getPosition(*window).x;
    y = sf::Mouse::getPosition(*window).y;
}

void drawCursor(sf::RenderWindow *window){
    sf::CircleShape circle(5);
    circle.setPosition(sf::Vector2f(x-5, y-5));
    circle.setFillColor(sf::Color::Red);
    window->draw(circle);
}

float vectorDistance(Eigen::Vector3d point1, Eigen::Vector3d point2){
    return sqrtf(powf(point1.x()-point2.x(), 2)+powf(point1.y()-point2.y(), 2));
}

void drawArm(sf::RenderWindow *window){
    for(int i = 0; i<NUM_SEG; i++){
        segments[i].draw(window, WIDTH, HEIGHT);
    }
}

void updateIK(Eigen::Vector3d endPos, sf::RenderWindow *window){
    Eigen::Vector3d rootPos,curEnd,desiredEnd,targetVector,curVector,crossResult;
    double cosAngle,turnDeg;
    int link,tries;

    link = NUM_SEG-1;
    tries = 0;

    do{
        // POSITION OF ROOT OF THIS SEGMENT
        rootPos = segments[link].getBegin();

        // POSITION OF THE END EFFECTOR
        curEnd = segments[NUM_SEG-1].getEnd();

        // DESIRED END EFFECTOR POSITION
        desiredEnd = endPos;

        if (vectorDistance(curEnd, desiredEnd) > IK_POS_THRESH){
            curVector.x() = curEnd.x() - rootPos.x();
            curVector.y() = curEnd.y() - rootPos.y();
            curVector.z() = curEnd.z() - rootPos.z();

            targetVector.x() = endPos.x() - rootPos.x();
            targetVector.y() = endPos.y() - rootPos.y();
            targetVector.z() = 0.0f;

            curVector.normalize();
            targetVector.normalize();

            cosAngle = targetVector.dot(curVector);

            if(cosAngle < 0.99999){
                crossResult = targetVector.cross(curVector);

                if(crossResult.z() > 0.0f){
                    turnDeg = ANGLE(acos((float)cosAngle)); // GET THE ANGLE

                    if(turnDeg+segments[link].getAngle() > 180){
                        turnDeg = -180 + turnDeg;
                    } else if(turnDeg+segments[link].getAngle() < 180){
                        turnDeg = 180 + turnDeg;
                    }

                    turnDeg = turnDeg;
                    segments[link].calculateEndPoint(rootPos, segments[link].getLength(), turnDeg);
                    //segments[link+1].setEnd(segments[link].getBegin());
                    printf("Angle: %f\r", turnDeg);
                }
                else if (crossResult.z() < 0.0f){
                    turnDeg = ANGLE(acos((float)cosAngle)); // GET THE ANGLE

                    if(turnDeg+segments[link].getAngle() > 180){
                        turnDeg = -180 + turnDeg;
                    } else if(turnDeg+segments[link].getAngle() < 180){
                        turnDeg = 180 + turnDeg;
                    }

                    turnDeg = turnDeg;
                    segments[link].calculateEndPoint(rootPos, segments[link].getLength(), turnDeg);
                    //segments[link+1].setEnd(segments[link].getBegin());
                    printf("Angle: %f\r", turnDeg);
                }

                drawArm(window);
            }
        }
        if (--link < 0) link = NUM_SEG - 1; // START OF THE CHAIN, RESTART

    }while(tries++ < MAX_IK_TRIES && vectorDistance(curEnd, desiredEnd) > IK_POS_THRESH);

}

int main() {

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Arm Simulator", sf::Style::Default);

    segments[0] = segment(Eigen::Vector3d(0, 0, 0), len);

    for(int i = 1; i<NUM_SEG; i++){
        segments[i] = segment(segments[i-1].getEnd(), len);
        segments[i].calculateEndPoint();
    }

    for(int i = 0; i<NUM_SEG; i++){
        printf("segment %i: A:(%.2f, %.2f), B:(%.2f, %.2f)\n", i, segments[i].getBegin().x(), segments[i].getBegin().y(), segments[i].getBegin().x(), segments[i].getBegin().y());
    }

    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event{};
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            else if( event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left )
                mouse_clicked = true;
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        updateIK(Eigen::Vector3d(x, y, 0), &window);

        if(mouse_clicked){
            printf("%s", "CLICK\n");
            updateCursorPosition(&window);
            mouse_clicked = false;
        }

        drawCursor(&window);

        // end the current frame
        window.display();
    }

    return 0;
}