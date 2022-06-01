#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

//Pin 11 on Raspberry Pi corresponds to BCM GPIO 17 and wiringPi pin 0
#define BeepPin 0

// Pİn 13 GPIO27 is wiring Pi pin 2
#define ButtonPin 16 //durdur devam
#define ButtonStopPin 5
#define ButtonReplayPin 3
#define ButtonResumePin 4 // will be used for play or resume

#define ButtonNextMusicPin 15 // next song
#define ButtonPreviousMusicPin 6

//FREQUENCIES
#define p 0
#define end 1

const int c[5] = {32, 65, 131, 262, 523};
const int db[5] = {34, 69, 139, 277, 554};
const int d[5] = {36, 73, 147, 294, 587};
const int eb[5] = {37, 78, 156, 311, 622};
const int e[5] = {41, 82, 165, 330, 659};
const int f[5] = {43, 87, 175, 349, 698};
const int gb[5] = {46, 92, 185, 370, 740};
const int g[5] = {49, 98, 196, 392, 784};
const int ab[5] = {52, 104, 208, 415, 831};
const int a[5]  = {55, 110, 220, 440, 880};
const int bb[5] = {58, 117, 223, 466, 932};
const int b[5] = {61, 123, 246, 492, 984};

//The source code of the Music Player
#define MAX_MUSIC 10
#define MAX_NOTE 256

int count = 0;
int playing = 0;

void play();
void stop();
void playNextMusic();
void playPreviousMusic();
int isLastMusic();
void handleAnyMusicChanges();

//This function generates the square wave that makes the piezo speaker sound at a determinated frequency.
void beep(unsigned int note, unsigned int beat, unsigned int tempo)
{
  digitalWrite(BeepPin, HIGH);
  delay(beat*tempo);
  digitalWrite(BeepPin, LOW);
  
  int c = 0;
  while(1){
	  int playButtonClicked = digitalRead(ButtonPin);
	  c++;
	  
	  if (playButtonClicked == 1)
	  {
		  if (playing == 1)
			stop();
			
		  break;
		}
	  
	  if (c == 1000)
		break;
	}
  
  delay(100);
}

int mailRequest(int musicIndex,char musicName[40]) {
	
	CURL *curl;
	CURLcode res;
	
	curl = curl_easy_init();
	printf("Curl yapılacak");
	
	if(curl) {
		char mesaj[100] = "%d. musik suanda dinleniyor: %s";
		sprintf(mesaj, "%d", musicIndex, "%s", musicName);
		char request[300] = "https://doktoradanis.net/deneme.php?mesaj=de&mail=talhabilal632@gmail.com&konu=Muzik&baslik=MP3%20G%C3%BCvenlik%20Sistemleri";
		curl_easy_setopt(curl, CURLOPT_URL, request);
		
		res = curl_easy_perform(curl);
		printf("Curl yapildi.");
		if(res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			}
		else {
			
			printf("RES: %s \n",res);
			}
			printf("Curl döndü");
			curl_easy_cleanup(curl);
			return 0;
	}
}

typedef struct music
{
  char name[40];
  int notes[MAX_NOTE][2];
  int tempo;
} music;

#define AVAILABLE_MUSIC_COUNT 3
music musics[AVAILABLE_MUSIC_COUNT] = {
	{"London Bridges", {{g[1], 2}, {a[1], 0.5}, {g[1], 1}, {f[1], 1}, {e[1], 1}, {f[1], 1}, {g[1], 2}, {d[1], 1}, {e[1], 1}, {f[1], 2}, {e[1], 1}, {f[1], 1}, 
		{g[1], 2}, {g[1], 2}, {a[1], 0.5}, {g[1], 1}, {f[1], 1}, {e[1], 1}, {f[1], 1}, {g[1], 2}, {d[1], 2}, {g[1], 2}, {e[1], 1}, {c[1], 1},{end, 0}}, 50},
	{"London Bridges 2", {{f[1], 2}, {f[2], 0.5}, {f[3], 1}, {f[0], 1}, {ab[1], 1}, {ab[1], 1}, {ab[1], 2}, {ab[1], 1}, {ab[1], 1}, {ab[1], 2}, {ab[1], 1}, {ab[1], 1}, 
		{g[1], 2}, {g[1], 2}, {a[1], 0.5}, {g[1], 1}, {f[1], 1}, {e[1], 1}, {f[1], 1}, {g[1], 2}, {d[1], 2}, {g[1], 2}, {e[1], 1}, {c[1], 1},{end, 0}}, 0.5},
	{"Eysan", {
    {g[2], 3}, {f[2], 0.25}, {g[2], 0.25}, {a[2], 0.25}, {f[2], 0.25}, {g[2], 0.25}, {f[2], 0.25},
    {g[2], 0.25}, {a[2], 0.25}, {f[2], 0.25}, {g[2], 0.5}, {e[2], 0.5}, {f[2], 0.5}, {d[2], 0.5}, {e[2], 0.5}, {c[2], 0.5},
    {f[2], 0.5}, {d[2], 0.5}, {p, 3}, {g[2], 1}, {f[2], 1}, {e[2], 2},
 
    {g[1], 3}, {f[1], 0.25}, {g[1], 0.25}, {a[1], 0.25}, {f[1], 0.25}, {g[1], 0.25}, {f[1], 0.25},
    {g[1], 0.25}, {a[1], 0.25}, {f[1], 0.25}, {g[1], 0.5}, {e[1], 0.5}, {f[1], 0.5}, {d[1], 0.5}, {e[1], 0.5}, {c[1], 0.5}, {c[1], 3*0.5}, {p, 2},
    {f[1], 0.5}, {d[1], 3*0.5}, {p, 2}, {g[1], 1}, {f[1], 1}, {e[1], 1}, {p, 1},
 
    {g[2], 3}, {f[2], 0.25}, {g[2], 0.25}, {a[2], 0.25}, {f[2], 0.25}, {g[2], 0.25}, {f[2], 0.25},
    {g[2], 0.25}, {a[2], 0.25}, {f[2], 0.25}, {g[2], 0.5}, {e[2], 0.5}, {f[2], 0.5}, {d[2], 0.5}, {e[2], 0.5}, {c[2], 0.5}, {p, 1},
    {f[2], 0.5}, {d[2], 0.5}, {p, 3}, {g[2], 1}, {f[2], 1}, {e[2], 2}, {end, 0}}, 0.195}
};

int currentMusicIndex = 0;

void replay()
{
  count = 0;
  play();
}

void resume()
{
  printf("muzik devam ediyor.");
  playing = 1;
}

void stop()
{
  printf("muzik durdu.");
  playing = 0;
}

struct mx{
    char x[40];
};

void play()
{
  playing = 1;
  music currentMusic = musics[currentMusicIndex];
  
  char musicName[40]; strcpy(musicName, currentMusic.name); 
  int notes[MAX_NOTE][2]; memcpy(notes, currentMusic.notes, sizeof(notes)); 
  printf("%d müzik başlayacak: %s \n", currentMusicIndex, musicName);
  mailRequest(currentMusicIndex, musicName);
  for (; count<MAX_NOTE; count++)
  {
    int isReplayButtonClicked = digitalRead(ButtonReplayPin);
    
    if (isReplayButtonClicked)
    {
        replay();
        break;
    }
      
    while(!playing)
    {

      int c = 0;
      while(1){
        int playButtonClicked = digitalRead(ButtonPin);
        c++;
        
        if (playButtonClicked == 1)
        {
          if (playing == 0)
          resume();
          
          break;
        }
        
        if (c == 1000)
        break;
      }
        
      isReplayButtonClicked = digitalRead(ButtonReplayPin);
      
      if (isReplayButtonClicked)
      {
          replay();
          break;
      }
      
    }
    
    if (isReplayButtonClicked)
      break;

    handleAnyMusicChanges();
    
    if (notes[count][0] == p)
    {
        delay(notes[count][1]);
        continue;
    }

    if (notes[count][0] == end && !isLastMusic())
    {
        playNextMusic();
        break;
    }
    
    if (notes[count][0] == end)
    {
      printf("muzik bitti\n");
      if (isLastMusic())
        currentMusicIndex = 0;
      break;
    }
        
    beep(notes[count][0], notes[count][1],notes[count][2]);
  }
}

void handleAnyMusicChanges()
{
  int isButtonNextMusicPinClicked = digitalRead(ButtonNextMusicPin);
  if (isButtonNextMusicPinClicked)
    playNextMusic();
  
  int isButtonPreviousMusicPinClicked = digitalRead(ButtonPreviousMusicPin);
  if (isButtonPreviousMusicPinClicked)
    playPreviousMusic();
}

void playNextMusic()
{
  delay(250);	
  currentMusicIndex++;
  if (currentMusicIndex >= AVAILABLE_MUSIC_COUNT)
    currentMusicIndex = 0;
    
  replay();
}

void playPreviousMusic()
{
  currentMusicIndex--;
  if (currentMusicIndex < 0)
    currentMusicIndex = AVAILABLE_MUSIC_COUNT - 1;
    
  replay();
}

int isLastMusic()
{
  return currentMusicIndex == AVAILABLE_MUSIC_COUNT-1;
}

int main(void)
{
  //Check wiringPi setup
  if(-1 == wiringPiSetup())
  {
    printf("setup wiringPi failed!");
    return 1;
  }

  //Prepare GPIO0
  pinMode(BeepPin, OUTPUT);
  
  pinMode(ButtonPin, INPUT);
  pinMode(ButtonStopPin, INPUT);
  pinMode(ButtonResumePin, INPUT);
  pinMode(ButtonReplayPin, INPUT);
  
  pullUpDnControl(ButtonPin,PUD_DOWN);
  
  printf("Press play button to start playing music\n");
  
  while(1)
  {
    delay(250);
    if(digitalRead(ButtonPin) == 1) {
        
        count = 0;
        play();
        digitalWrite(BeepPin, LOW);
    
    }
  }

  return 0;
}
