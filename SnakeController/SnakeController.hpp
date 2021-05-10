#pragma once

#include <list>
#include <memory>
#include <stdexcept>
#include <functional>

#include "IEventHandler.hpp"
#include "SnakeInterface.hpp"

class Event;

class IPort;

namespace Snake {
    struct ConfigurationError : std::logic_error
    {
        ConfigurationError();
    };

    struct UnexpectedEventException : std::runtime_error
    {
        UnexpectedEventException();
    };
    class ISegment
    {
        virtual bool isSegmentAtPosition(int x, int y) const = 0;
    };
    class Segment : public ISegment
    {
    protected:
        bool isSegmentAtPosition(int x, int y) const override;

        struct snake
        {
            int x;
            int y;
        };
        std::list<snake> m_segments;

    };

    class Controller : public IEventHandler, public Segment
    {
    public:
        Controller(IPort &p_displayPort, IPort &p_foodPort, IPort &p_scorePort, std::string const &p_config);

        Controller(Controller const &p_rhs) = delete;

        Controller &operator=(Controller const &p_rhs) = delete;

        void receive(std::unique_ptr<Event> e) override;


    private:
        IPort &m_displayPort;
        IPort &m_foodPort;
        IPort &m_scorePort;

        std::pair<int, int> m_mapDimension;
        std::pair<int, int> m_foodPosition;

        Direction m_currentDirection;

        snake calculateNewHead() const;

        void updateSegmentsIfSuccessfullMove(snake const &newHead);

        void handleDirectionInd(std::unique_ptr<Event>);

        void handleFoodInd(std::unique_ptr<Event>);

        void handleFoodResp(std::unique_ptr<Event>);

        void handlePauseInd(std::unique_ptr<Event>);

        void handleTimeoutInd();


        void addHeadSegment(snake const &newHead);

        void removeTailSegmentIfNotScored(snake const &newHead);

        void removeTailSegment();

        bool isPositionOutsideMap(int x, int y) const;

        void updateFoodPosition(int x, int y, const std::function<void()> &clearPolicy);

        void sendClearOldFood();

        void sendPlaceNewFood(int x, int y);

        bool m_paused;
    };


} // namespace Snake
