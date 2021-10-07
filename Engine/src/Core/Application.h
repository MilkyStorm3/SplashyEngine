#pragma once

namespace ant
{

    class Application
    {

    public:
        ~Application(){}

        virtual void Init();
        virtual void Run();

    protected:
        Application() {}
        
    private:
        struct
        {
            bool running = true;
        } m_appdata;
    };

}