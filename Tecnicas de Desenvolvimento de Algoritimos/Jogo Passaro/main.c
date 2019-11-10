#include <allegro.h>

#define VELOCIDADE 35

#define X_PASSARO 250
#define SALTO_PASSARO 15;
#define GRAVIDADE 3;
#define ACELERA_GRAVIDADE 8;
#define VELOCIDADE_OBSTACULOS 10;

#define Y_CHAO 410

typedef struct { 
    int x; 
    int y;
    int w;
    int h;
    float angulo;
} obstaculo;

struct {
    int x;
    int y;
    int comprimento;
    int altura;
    float angulo;
    int tempo;
    int tempoAsa;
    BITMAP *bmp; 
} passaro;


void sair_allegro();
void init();
void deinit();
int  colidir(int Ax,int Ay,int Aw,int Ah, obstaculo *ob, int *placar);
void iniciaObstaculos(obstaculo *ob, int espaco);

int sair = 0;

int main() {
	init();
    
    int i;
	int x_chao = 0, t = 0, subir = 0;
	int colidiu = 0;
	int placar = 0;
	int status = 0;
	int tpObstaculo;
	
	//cria a imagem onde vai ser redesenhado as outras imagens
	BITMAP *buffer = create_bitmap(640, 480);
	
    //carrega as imagens:
    BITMAP *chao = load_bitmap("imagens\\chao.bmp",NULL);
    BITMAP *cano = load_bitmap("imagens\\cano.bmp",NULL);
    BITMAP *p1 = load_bitmap("imagens\\passaro1.bmp",NULL);
    BITMAP *p2 = load_bitmap("imagens\\passaro2.bmp",NULL);
    FONT *font1 = load_font("fontes\\ArialBlack.pcx",NULL,NULL);
    
    //carrega os sons 
    SAMPLE  *piu = load_sample("sons\\piu.wav");
    
    //inicializa as variaveis do passaro
    passaro.x = 250;
	passaro.y = 100;
	passaro.comprimento = 42;
	passaro.altura = 34;
	passaro.angulo = 0;
	passaro.tempoAsa = 10;
	
	//cria o array de array de 6 strunct do tipo obstaculo
	obstaculo ob[6];
	iniciaObstaculos(ob,220);
	
	//roda o aplicativo enquento nao for clicado no botao sair
	while (!sair) { 
        //se clicar o botao ESC fechar o programa tambem  
        if (key[KEY_ESC]) sair = 1;  
        
        //cria o fundo azul da tela
        rectfill(buffer,0,0,640,480,makecol(26, 185, 198));
            
		//movimenta o chao
        if(x_chao < -639) x_chao = -10;
        x_chao-=6;
        
        //alterna as imagens dos passaros a cada TEMPO_ASA.
        ++t;
        if(t > passaro.tempoAsa) {
            passaro.bmp = p1;
            if(t == 2 * passaro.tempoAsa) t = 0;
        }
        else {
             passaro.bmp = p2;
        }
        
        switch(status){
            case 0:{ //tela do nome do jogo 
              //reinicia obstaculos e placar.
              
              placar = 0;
              //remove a colisao
              colidiu = 0;
              
              //desenha o chao em movimento
              draw_sprite(buffer,chao,x_chao,Y_CHAO);
              
              //reposiciona o passaro
              passaro.x = 250; 
              passaro.angulo = 0;
              
              if (subir != 0){ 
                 passaro.y -= 2;
              }
              else {
                 passaro.y += 2;
              }
              
              if(passaro.y > 200){
                 subir = 1;
              }
              else {
                 if(passaro.y < 100) subir = 0;
              }
              
              if (key[KEY_SPACE]) {
                  while(key[KEY_SPACE]); 
                  passaro.tempoAsa = 1;
                  status = 1;
              }     
              break;
            }
            case 1:{ //tela de começar 
              
              textout_centre_ex(buffer, font1, "COMECAR!!" ,SCREEN_W / 2, 20, makecol(255, 225, 255), -1);
              
              //reinicia placar.
              placar = 0;
              //remove a colisao
              colidiu = 0;
              
              //reposiciona o passaro
              passaro.x = 250; 
              passaro.angulo = 0;
              
              //desenha o chao em movimento
              draw_sprite(buffer,chao,x_chao,Y_CHAO);
              
              if (key[KEY_SPACE]) {
                  while(key[KEY_SPACE]); 
                  passaro.tempoAsa = 1;
                  status = 2;
              }     
              break;
            }
            case 2:{//jogo iniciado
                 
        		//passaro.tempoAsa = 1;
                //posiciona os obstaculos
        		for(i = 0; i < 6; i+=2) {
                    if(ob[i].x < -900 || (ob[i].x > -300 && ob[i].x < -200) ) {
                       //posiciona os dois canos a direita da tela
                       ob[i].x = 720; 
                       ob[i+1].x = 720;
                            
                       //existem tres obstaculos: passagem por baixo, pelo meio e por cima            
                       //sorteia os obstaculos:
                       tpObstaculo = rand() %3;          
                       switch (tpObstaculo){
                       case 0: //por baixo
                            ob[i].y = 350;//cano de baixo 350
                            ob[i+1].y = -50; //cano de cima -20
                            break;
                       case 1: //pelo meio
                            ob[i].y = 250; //250
                            ob[i+1].y = -140; //-110
                            break;
                       case 2: //por cima
                            ob[i].y = 140; //140
                            ob[i+1].y = -250; //-220
                            break;
                       }
                       
                    }
                    rotate_sprite(buffer,cano,ob[i].x,ob[i].y,itofix(ob[i].angulo));
                    rotate_sprite(buffer,cano,ob[i+1].x,ob[i+1].y,itofix(ob[i+1].angulo));
                }
                
                if(!colidiu){
                    if(passaro.y > (Y_CHAO - passaro.altura)) {
                         colidiu  = 1;
                    }
                    else {
                         colidiu  = colidir(passaro.x,passaro.y,passaro.comprimento,passaro.altura, ob, &placar);
                    }
                }
                
                char str[12];
                sprintf(str, "%d", placar);
                textout_centre_ex(buffer, font1, str ,SCREEN_W / 2, 20, makecol(255, 225, 255), -1);
         		
        		if(colidiu){ 
                   //faz o passaro descer apos colodir.
                   if(passaro.y < (Y_CHAO - passaro.altura)) passaro.y+=10;
                   if(passaro.y > (Y_CHAO - passaro.altura)) passaro.y = Y_CHAO - passaro.altura;
                   
                   passaro.bmp = p1;                                                                                                              
                   draw_sprite(buffer,chao,0,Y_CHAO);
                   passaro.angulo = 60;
                   textout_centre_ex(buffer, font, "GAME OVER",SCREEN_W / 2, SCREEN_H / 2, makecol(255, 0, 0), makecol(0, 0, 0));
                   
                   if (key[KEY_SPACE]){
                      //espera a tecla ser desprecionada                 
                      while(key[KEY_SPACE]); 
                      passaro.tempoAsa = 10;
                      passaro.y = 100;
                      status = 0;
                      iniciaObstaculos(ob,220);
                   }
                }
                else {   
            		//movimenta os obstaculos
            		for(i = 0; i < 6; i++) ob[i].x -= VELOCIDADE_OBSTACULOS;
                    
                    draw_sprite(buffer,chao,x_chao,Y_CHAO);
                    
                    passaro.y += GRAVIDADE;
                    for(i = 0; i < 6; i+=2) {
                          if (passaro.x ==  (ob[i].x + (ob[i].w /2))) placar++;
                    }
                     
                    if(key[KEY_SPACE]){
                       passaro.y-= SALTO_PASSARO;
                       if(passaro.y < 0) passaro.y = 0;
                       //passaro.angulo = -15;
                       passaro.tempo = 0;
                       //play_sample(piu,255,128,1000,0);
                    }
                    else {
                        if(passaro.tempo == 5) {   
                            //passaro.angulo += 20;
                            //if(passaro.angulo > 60) passaro.angulo = 60;
                            //if(passaro.angulo == 60) passaro.y += 8;
                            passaro.y += ACELERA_GRAVIDADE;
                        }
                        else {
                            passaro.tempo++;  
                        } 
                    }	
                    
                }
                break;
            }
        }
        
        rotate_sprite(buffer,passaro.bmp,passaro.x,passaro.y,itofix(passaro.angulo));
        rest(VELOCIDADE);
        //show_mouse(buffer);
		draw_sprite(screen,buffer,0,0);
		
		clear(buffer);
	}
	destroy_bitmap(cano);
    destroy_bitmap(p1);
	destroy_bitmap(p2);
	destroy_bitmap(chao);
	destroy_bitmap(buffer);
	destroy_font(font1);
	destroy_sample(piu);
	deinit();
	return 0;
}
END_OF_MAIN()

void sair_allegro(){
   sair = 1;
}

END_OF_FUNCTION(sair_allegro)

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	set_close_button_callback(sair_allegro);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}
	set_window_title("drunk bird");

	install_timer();
	install_keyboard();
	install_mouse();
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT,NULL);
	
	/* add other initializations here */
}

void deinit() {
	clear_keybuf();
	/* add other deinitializations here */
}

int colidir(int Ax,int Ay,int Aw,int Ah, obstaculo *ob, int *placar){
    int i;
    for(i = 0; i < 6; i++) {
       
       if(Ax + Aw > ob[i].x && Ax < ob[i].x + ob[i].w && Ay + Ah > ob[i].y && Ay < ob[i].y + ob[i].h) return 1; 
       
       //*placar = ob[0].x;
            
    }
    return 0;
}

void iniciaObstaculos(obstaculo *ob, int espaco){
    int i,pos = espaco + 80;
    for(i = 5; i >= 0; i-=2) {
       //obstaculo de cima
       ob[i].x = pos * -1;
       ob[i].angulo = 128;
       ob[i].w = 80;
       ob[i].h = 287;
       //obstaculo de de baixo
       ob[i-1].x = pos * -1;
       ob[i-1].w = 80;
       ob[i-1].h = 287;
       pos += espaco + 80;
    }
}
