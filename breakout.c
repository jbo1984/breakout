//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// height and width of paddle in pixels
#define PADDLEH 30
#define PADDLEW 70

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    // sets velocity and angle of ball
    double velocity = drand48()*2;
    double angle = drand48()*1.5;
    
    //Keeps playing till no bricks or lives are left
    
    while (lives > 0 && bricks > 0)
    {    

        // check for mouse event
        GEvent event = getNextEvent(MOUSE_EVENT);

        // if we heard one
        if (event != NULL)
        {
            // if the event was movement
            if (getEventType(event) == MOUSE_MOVED)
            {   
                double x = getX(event)-PADDLEW/2;
                double y = HEIGHT - PADDLEH;
                
                setLocation(paddle, x, y);
            }
        }
        
        
                move(ball, velocity, angle);
                GObject object = detectCollision(window, ball);
                // bounce off right edge of window
                if (getX(ball) + getWidth(ball) > 400)
                {
                    
                    velocity = -velocity;
                }
                // bounce off left edge of window
                else if (getX(ball) <= 0)
                {
                    velocity = -velocity;
                }
                
                else if(object == paddle)
                {
                    angle = -angle;
                }
                else if(getY(ball) + getHeight(ball) > HEIGHT)
                {
                    velocity = 0;
                    angle = 0;
                    lives -= 1;
                    //break; 
                }
                else if (getY(ball) <= 0)
                {
                    angle = -angle;
                }

                // linger before moving again
                pause(5);
        }
    

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            
            
            GRect rect = newGRect(5 + (80*i), 5+(25*j) , 70, 20);
            setFilled(rect, true);
            setColor(rect, "GREEN");
            add(window, rect); 
            
        }
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    
    GOval ball = newGOval(185, 285, 30, 30);
    setColor(ball, "BLUE");
    setFilled(ball, true);
    add(window, ball);
    
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    //GRect  newGRect(double x, double y, double width, double height);
    //void drawRect(GWindow gw, double x, double y, double width, double height);
    double xStart = (WIDTH/2)-(PADDLEW /2);
    double yStart = HEIGHT - PADDLEH;
    
    GRect rect = newGRect(xStart, yStart , PADDLEW, PADDLEH);
    setFilled(rect, true);
    setColor(rect, "RED");
    add(window, rect); 
    return rect;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // TODO
    return NULL;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
