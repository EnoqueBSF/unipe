#include <allegro.h>
#define VELOCIDADE 30
#define TEMPO_ASA 10
#define X_PASSARO 250

void init();
void deinit();

int main() {
	init();
	int x = 0, t = 0, y_passaro = 100;
    BITMAP *fundo = load_bitmap("fundo.bmp",NULL);
    BITMAP *chao = load_bitmap("chao.bmp",NULL);
    BITMAP *p1 = load_bitmap("imagens\\passaro1.bmp",NULL);
    BITMAP *p2 = load_bitmap("imagens\\passaro2.bmp",NULL);
    BITMAP *cano = load_bitmap("imagens\\cano.bmp",NULL);
    BITMAP *buffer = create_bitmap(640, 480);
	while (!key[KEY_ESC]) {     
		if(x < -640) x = 0;
        x-=6;
        
        
		draw_sprite(buffer,fundo,0,0);
		
        //rotate_sprite(buffer,cano,200,300,itofix(0));
		
		
		if(y_passaro > 369) {
           draw_sprite(buffer,chao,0,401);
           rotate_sprite(buffer,p1,X_PASSARO,y_passaro,itofix(60));
           textout_centre_ex(buffer, font, "GAME OVER",
                        SCREEN_W / 2, SCREEN_H / 2,
                        makecol(255, 0, 0), makecol(0, 0, 0));
        }
        else {   
    		draw_sprite(buffer,chao,x,401);
            t++;
            y_passaro += 3;
            
             
            if(key[KEY_SPACE]){
               y_passaro-=10;
               if(y_passaro < 0) y_passaro = 0;
               rotate_sprite(buffer,p1,X_PASSARO,y_passaro,itofix(-15));
            }
            else {
                if (t > TEMPO_ASA){
                     rotate_sprite(buffer,p1,X_PASSARO,y_passaro,itofix(30));
                     if(t == TEMPO_ASA) t = 0;
                }
                else {
                     rotate_sprite(buffer,p2,X_PASSARO,y_passaro,itofix(30));
                }
            }	
        }
        rest(VELOCIDADE);
		draw_sprite(screen,buffer,0,0);
		
		clear(buffer);
	}
	destroy_bitmap(cano);
	destroy_bitmap(fundo);
	destroy_bitmap(chao);
	destroy_bitmap(buffer);
	deinit();
	return 0;
}
END_OF_MAIN()

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}

	install_timer();
	install_keyboard();
	install_mouse();
	/* add other initializations here */
}


void deinit() {
	clear_keybuf();
	/* add other deinitializations here */
}
