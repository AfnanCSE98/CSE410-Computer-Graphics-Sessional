#include<bits/stdc++.h>
#include "zbuffer.h"
#include "input_handler.h"

using namespace std;

int main(){
   
    InputHandler inputHandler = InputHandler("scene.txt");
    inputHandler.processInput();
    zbuffer zb = zbuffer("config.txt", "Outputs/stage3.txt");
    zb.apply_procedure();
    zb.generate_output();
}