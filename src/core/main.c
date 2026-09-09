#include "dmagh_layer.h"
int main(){
    application* app = create_gaven_application();
    app->Running=1;
    add_layer(app->Layer_Registry,create_dmagh_layer());
    run_application();
    destroy_application();
}