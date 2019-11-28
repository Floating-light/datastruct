#pragma once

class DefalutSynthesized
{
    public:
    //copy control, using defalut
    DefalutSynthesized() = default;//explicitly ask compiler to generate the synthesized version copy-control members.
                                   //implicitly inline.
    DefalutSynthesized(const DefalutSynthesized&) = default;
    DefalutSynthesized& operator=(const DefalutSynthesized&);
    ~DefalutSynthesized() = default;
};

//aviod inline.
DefalutSynthesized& DefalutSynthesized::operator=(const DefalutSynthesized& ) = default;