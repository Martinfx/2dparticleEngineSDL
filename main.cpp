#include <iostream>
#include <stdint.h>
#include <list>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <pugixml.hpp>

#include "module.hpp"
#include "window.hpp"
#include "engine.hpp"
#include "input.hpp"


    enum MainState
    {
        CREATE = 1,
        AWAKE,
        START,
        LOOP,
        CLEAN,
        FAIL,
        EXIT
    };


int main(int argc, char* args[])
{
       // LOG("Engine starting ... %d");
        Engine* Application = NULL;
        MainState state = MainState::CREATE;
        int result = EXIT_FAILURE;

        while(state != EXIT)
        {
            switch(state)
            {


            case CREATE:
                std::cerr << ("CREATION PHASE ===============================");

                Application = new Engine(argc, args);

                if(Application != NULL)
                    state = AWAKE;
                else
                    state = FAIL;

                break;

                // Awake all modules -----------------------------------------------
            case AWAKE:
                 std::cerr << ("AWAKE PHASE ===============================");
                if(Application->Awake() == true)
                    state = START;
                else
                {
                    //LOG("ERROR: Awake failed");
                    state = FAIL;
                }

                break;

                // Call all modules before first frame  ----------------------------
            case START:
                 std::cerr <<  "START PHASE ===============================";
                if(Application->Start() == true)
                {
                    state = LOOP;
                   std::cerr <<  "UPDATE PHASE ===============================";
                }
                else
                {
                    state = FAIL;
                    std::cerr <<  "ERROR: Start failed";
                }
                break;

                // Loop all modules until we are asked to leave ---------------------
            case LOOP:
                if(Application->Update() == false)
                    state = CLEAN;
                break;

                // Cleanup allocated memory -----------------------------------------
            case CLEAN:
                 std::cerr << "CLEANUP PHASE ===============================";
                if(Application->CleanUp() == true)
                {
                    delete Application;
                    result = EXIT_SUCCESS;
                    state = EXIT;
                }
                else
                    state = FAIL;

                break;

                // Exit with errors and shame ---------------------------------------
            case FAIL:
                std::cerr << "Exiting with errors :(";
                delete Application;
                result = EXIT_FAILURE;
                state = EXIT;
                break;
            }
        }

        //LOG("... Bye! :)\n");

        // Dump memory leaks
        return result;
}

