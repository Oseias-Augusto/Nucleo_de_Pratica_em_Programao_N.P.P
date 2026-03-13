#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>

int main (){

  // Inicializar Libs
  al_init();
  al_init_font_addon();
  al_init_image_addon();
  al_install_keyboard();

  // Criar Display
  ALLEGRO_DISPLAY * display = al_create_display(1280,960);
  al_set_window_position(display, 400, 100);

  // Font e FPS
  ALLEGRO_FONT* font = al_create_builtin_font();
  ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);

  // Sprites
  ALLEGRO_COLOR masc = al_map_rgb(64, 202, 201);
  ALLEGRO_BITMAP* dino = al_load_bitmap("./Sprites/t-rex.png");
  al_convert_mask_to_alpha(dino, masc);

  ALLEGRO_BITMAP* map = al_load_bitmap("./Sprites/misse.png");
  al_convert_mask_to_alpha(map, masc);

  // Fila de Eventos
  ALLEGRO_EVENT_QUEUE * event_queue = al_create_event_queue();
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_start_timer(timer);

  // Variaveis de estado
  int inicio = 0; // Play do jogo
  int posi_y = 480; // Posicao inicial
  int chao = 480; // chao
  int pulando = 0; // Estado de pualndo
  float vel_y = 0; // Velocidade vertical
  float gravidade = 3; // Puxar o Personagem para baixo
  int sprite_atual = 0; // para mudar o sprite (100w)

  // Loop de jogo
  while(true){
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    if( event.type == ALLEGRO_EVENT_DISPLAY_CLOSE ){
      break;
    } else if((event.keyboard.keycode == ALLEGRO_KEY_SPACE) && (inicio == 0)){
      sprite_atual += 100;
      inicio = 1;
    } 
    
    // Botões
    if (event.type == ALLEGRO_EVENT_KEY_DOWN){
        if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && !pulando){
          vel_y -= 30;
          pulando = 1;
        }
    }
    
    posi_y += vel_y;
    vel_y += gravidade;

    if (posi_y >= chao) {
      posi_y = chao;
      vel_y = 0;
      pulando = 0;
    }


    al_clear_to_color(al_map_rgb(255,255,255));
    //al_draw_text(font, al_map_rgb(0, 0, 0), 230, 200, 0, "Allegro is working!");
    al_draw_bitmap_region(dino, sprite_atual, 55, 95, 100, 50, posi_y, 0);
    al_flip_display();
  }

  // Limpando memoria
  al_destroy_font(font);
  al_destroy_bitmap(dino);
  al_destroy_bitmap(map);
  al_destroy_display(display);
  al_destroy_event_queue(event_queue);

  return 0;
}