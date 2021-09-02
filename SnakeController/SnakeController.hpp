#pragma once

#include <list>
#include <memory>
#include <functional>
#include <stdexcept>

#include "IEventHandler.hpp"
#include "SnakeInterface.hpp"

class Event;
class IPort;

namespace Snake
{
struct ConfigurationError : std::logic_error
{
    ConfigurationError();
};

struct UnexpectedEventException : std::runtime_error
{
    UnexpectedEventException();
};

class Controller : public IEventHandler
{
public:
    Controller(IPort& p_displayPort, IPort& p_foodPort, IPort& p_scorePort, std::string const& p_config);

    Controller(Controller const& p_rhs) = delete;
    Controller& operator=(Controller const& p_rhs) = delete;

    void receive(std::unique_ptr<Event> e) override;

private:
    IPort& m_displayPort;
    IPort& m_foodPort;
    IPort& m_scorePort;

    std::pair<int, int> m_mapDimension;
    std::pair<int, int> m_foodPosition;

    struct Segment
    {
        int x;
        int y;
    };

    std::list<Segment> m_segments;
    Direction m_currentDirection;

    void handleTimeoutInd();
    void handleDirectionInd(std::unique_ptr<Event>);
    void handleFoodInd(std::unique_ptr<Event>);
    void handleFoodResp(std::unique_ptr<Event>);
    void handlePauseInd(std::unique_ptr<Event>);

    bool isSegmentAtPosition(int x, int y) const;
    Segment calculateNewHead() const;
    void updateSegmentsIfSuccessfullMove(Segment const& newHead);
    void addHeadSegment(Segment const& newHead);
    void removeTailSegmentIfNotScored(Segment const& newHead);
    void removeTailSegment();

    bool isPositionOutsideMap(int x, int y) const;

    void updateFoodPosition(int x, int y, std::function<void()> clearPolicy);
    void sendClearOldFood();
    void sendPlaceNewFood(int x, int y);

    bool m_paused;
};

class Segments : public IEventHandler, public Controller{
public:
    Segments(IPort& p_displayPort, IPort& p_foodPort, IPort& p_scorePort, std::string const& p_config);

    Segments(Segments const& p_rhs) = delete;
    Segments& operator=(Segments const& p_rhs) = delete;

    void receive(std::unique_ptr<Event> e) override;
    void handleTimePassed(const TimeoutInd&);
    void handleDirectionChange(const DirectionInd& directionInd);
    bool doesCollideWithSnake(const Controller::Segment &newSegment) const;
    bool doesCollideWithWall(const Controller::Segment &newSegment) const;
    bool doesCollideWithFood(const Controller::Segment &newHead) const; 
    void repaintTile(const Controller::Segment &position, Cell type);

private:
    IPort& m_displayPort;
    IPort& m_foodPort;
    IPort& m_scorePort;

    std::pair<int, int> m_mapDimension;
    std::pair<int, int> m_foodPosition;

    struct Segment
    {
        int x;
        int y;
    };

    std::list<Segment> m_segments;
    Direction m_currentDirection;
};

class World : public IEventHandler, public Controller{
    World(IPort& p_displayPort, IPort& p_foodPort, IPort& p_scorePort, std::string const& p_config);

    World(World const& p_rhs) = delete;
    World& operator=(World const& p_rhs) = delete;

    void receive(std::unique_ptr<Event> e) override;
    void handleFoodPositionChange(const FoodInd& receivedFood);
    void handleNewFood(const FoodResp& requestedFood);
    void notifyAboutFailure();

private:
    IPort& m_displayPort;
    IPort& m_foodPort;
    IPort& m_scorePort;

    std::pair<int, int> m_mapDimension;
    std::pair<int, int> m_foodPosition;

    struct Segment
    {
        int x;
        int y;
    };

    std::list<Segment> m_segments;
    Direction m_currentDirection;
};

} // namespace Snake
