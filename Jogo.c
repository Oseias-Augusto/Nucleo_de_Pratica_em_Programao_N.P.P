#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <stdlib.h>
#include <stdio.h>

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

  // Icone do Jogo
  // ALLEGRO_BITMAP * icon = al_load_bitmap("icon.png");
  // if (icon) {
  //     al_set_display_icon(display, icon);
  // }

  al_set_window_title(display, "Dino Game");

  // Cor da mascara para o spritesheet
  ALLEGRO_COLOR masc = al_map_rgb(64, 202, 201);

  // Dinossauro 
  ALLEGRO_BITMAP* dino = al_load_bitmap("./Sprites/t-rex.png");
  al_convert_mask_to_alpha(dino, masc);

  // Chão
  ALLEGRO_BITMAP* map = al_load_bitmap("./Sprites/misse.png");
  al_convert_mask_to_alpha(map, masc);

  // obst
  ALLEGRO_BITMAP* obst = al_load_bitmap("./Sprites/obstaculos.png");
  al_convert_mask_to_alpha(obst, masc);

  // Fila de Eventos
  ALLEGRO_EVENT_QUEUE * event_queue = al_create_event_queue();
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_start_timer(timer);

  
// Variavei de jogo
  int inicio = 0; // Play do jogo
  int fim = 0;
  float score = 0.0f;
  char score_text[50];

// Variaveis do Jogador
  int posi_y = 480; // Posicao inicial Vertical
  int posi_x = 50; // Posicao inicial Horizontal
  int chao = 480; // chao
  int pulando = 0; // Estado de pualndo
  float vel_y = 0; // Velocidade vertical
  float gravidade = 3; // Puxar o Personagem para baixo
  int sprite_atual = 0; // para mudar o sprite (95w)
  float frame = 1.0f; // Para trocar os frames de melhor forma

// Variaveis do Mapa
  int C_pos_x = 0; // Posição inicial do chão
  int vel_x = 0; // Velocidade Horizontal
  int largura = 2400; // Largura do mapa

// Obstaculos
  int obs_pos_x[3] = {1280, 1600, 2000};
  int obs_pos_y = 480;

  

// Loop de jogo
  while(true){
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    if( event.type == ALLEGRO_EVENT_DISPLAY_CLOSE ){
      break;
    } else if((event.keyboard.keycode == ALLEGRO_KEY_SPACE) && (inicio == 0)){
    // Iniciar o Jogo
      sprite_atual += 95;
      frame = 1;
      inicio = 1;
      vel_x -= 15;
    } 

    
    
  // Botões
    if (event.type == ALLEGRO_EVENT_KEY_DOWN){
        if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && !pulando){
          vel_y -= 30;
          pulando = 1;
          sprite_atual = 95;
          frame = 1;
        }
    }
    
  // Pular
    posi_y += vel_y;
    vel_y += gravidade;
    if (posi_y >= chao) {
      posi_y = chao;
      vel_y = 0;
      pulando = 0;
    }

  // Andar
    if (pulando != 1){
      frame += 0.3f;
      if( frame > 5){
        frame -= 2;
      }
    }

  // Mover Mapa e Obstaculos
    C_pos_x += vel_x;
    if(C_pos_x <= -largura){
      C_pos_x = 0;
    }

  // Display de  Sprites e tela
    al_clear_to_color(al_map_rgb(255,255,255));

    if(inicio == 1){
      score += 0.3f;
    }  
    sprintf(score_text, "Score: %.0f", score);
    al_draw_text(font, al_map_rgb(0, 0, 0), 20, 180, 0, score_text);

  // Dinossauro
    al_draw_bitmap_region(dino, sprite_atual * (int)frame, 55, 95, 100, posi_x, posi_y, 0);

  // Chão
    if (inicio == 1) {\
      al_draw_bitmap_region(map, 10, 30, largura, 25, C_pos_x, 555, 0);
      al_draw_bitmap_region(map, 10, 30, largura, 25, C_pos_x + largura, 555, 0);
    }

  // Obstaculos
    for( int i = 0; i < 3; i++ ){
      obs_pos_x[i] += vel_x;

      if(obs_pos_x[i] <= -40) {
        obs_pos_x[i] = 1400 + rand() % 400;
      }

    // Obstaculo
      al_draw_bitmap_region(obst, 5, 200, 40, 95, obs_pos_x[i], obs_pos_y, 0);

    // Colisão
      if (posi_x < obs_pos_x[i] + 40 &&
          posi_x + 95 > obs_pos_x[i] &&
          posi_y < obs_pos_y + 100 &&
          posi_y + 100 > obs_pos_y) {
          // Colisão detectada
          fim = 1;
      }
    }

    if(fim == 1) {
      break;
    }
    al_flip_display();
  }

  // Limpando memoria
  al_destroy_font(font);
  al_destroy_bitmap(dino);
  al_destroy_bitmap(map);
  al_destroy_bitmap(obst);
  al_destroy_display(display);
  al_destroy_event_queue(event_queue);

  return 0;
}