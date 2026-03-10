#pragma once
class App;

class Screen {
public:
    virtual ~Screen() = default;
    virtual void render(App& app) = 0;
};