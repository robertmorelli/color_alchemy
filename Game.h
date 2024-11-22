#ifndef GAME_H
#define GAME_H
#include <Box2D/Box2D.h>

class Game
{
public:
    Game();

private:
    b2Vec2 gravity;
    b2World world;
    b2Body* groundBody;

};

#endif // GAME_H
