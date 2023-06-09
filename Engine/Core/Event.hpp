#pragma once
#include "pch.hpp"

namespace Engine
{
    class Event
    {
    public:
        enum Type
        {
            None, WindowClose, WindowResize,
            KeyDown, KeyUp, MouseMove,
        };
    public:
        virtual Type GetType() const { return Type::None; }
    public:
        bool handled = false;
    };
    class EventWindowClose : public Event
    {
    public:
        EventWindowClose() { }
        virtual Type GetType() const override { return Type::WindowClose; }
    };
    class EventWindowResize : public Event
    {
    public:
        EventWindowResize(int width, int height) { m_width = width; m_height = height; }
        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }
        virtual Type GetType() const override { return Type::WindowResize; }
    private:
        int m_width, m_height;
    };
    class EventKeyDown : public Event
    {
    public:
        EventKeyDown(int keycode) { m_keycode = keycode; }
        int GetKeycode() const { return m_keycode; }
        virtual Type GetType() const override { return Type::KeyDown; }
    private:
        int m_keycode;
    };
    class EventKeyUp : public Event
    {
    public:
        EventKeyUp(int keycode) { m_keycode = keycode; }
        int GetKeycode() const { return m_keycode; }
        virtual Type GetType() const override { return Type::KeyUp; }
    private:
        int m_keycode;
    };
    class EventMouseMove : public Event
    {
    public:
        EventMouseMove(int x, int y, int offset_x, int offset_y) { m_x = x; m_y = y; m_offset_x = offset_x; m_offset_y = offset_y; }
        int GetX() const { return m_x; }
        int GetY() const { return m_y; }
        int GetOffsetX() const { return m_offset_x; }
        int GetOffsetY() const { return m_offset_y; }
        virtual Type GetType() const override { return Type::MouseMove; }
    private:
        int m_x, m_y, m_offset_x, m_offset_y;
    };
}