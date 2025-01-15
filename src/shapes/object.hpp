#ifndef SPACESTUFF_OBJECT_HPP
#define SPACESTUFF_OBJECT_HPP

class Object {
    public:
        virtual void draw() = 0;
        virtual void debug() = 0;
};

#endif