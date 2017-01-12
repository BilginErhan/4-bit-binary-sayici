#include <avr/io.h>//avr kütüphanesi
#include <avr/interrupt.h>//kesme kütüphanesi
void loopa_gir();
void reset();
volatile int durum=0;//start stop değişkeni
volatile int sayac=1;//sayi sayma
int main(void)
{
  cli();//kestinler kapatılır
  DDRB=B111100;//10-13 arasındaki pinlerdeki ledler output olarak tanımlandı
  DDRC=B011111;//0-4 arasındaki analog pinler input olarak tanımlandı
    
  TCCR1B|=(1<<WGM12);//timer1 ctc modda 
  TCCR1B|=(1<<CS12);//clk değeri 1024 olarak ayarlanır
  TCCR1B|=(1<<CS10);
  OCR1A=11718.75;//her 0.75 saniyede bir ISR fonksiyonunu tetiklemesini ister
  TIMSK1|=(1<<OCIE1A);
  
  sei();//kesmeleri tekrardan başlatır
  while(1)
  {
    loopa_gir();
  }
}
void loopa_gir()
{// arduino çalıştığı sürece sürekli dönen fonksiyon 
  if(PINC&(1<<PC0))
  {//eğer buton 1 e basılırsa start değişkeni birlenir
      durum=1;
  }
  if(PINC&(1<<PC1))
  {//eğer buton 2 e basılırsa start değişkeni sıfırlanır
    //ve durdururlur
    durum=0;
  }
  if(PINC&(1<<PC2))
  {
    
    OCR1A=OCR1A-1000;
    if(OCR1A<7813)
    {//bekleme süresi 0.5 saniyenin altına düşmicek
      OCR1A=7812.5;
    }
    TCNT1=0;
    
  }
  if(PINC&(1<<PC3))
  {
    
    OCR1A=OCR1A+1000;
    if(OCR1A>=15625)
    {//bekleme süresi 1 saniyeyi aşmıcak
      OCR1A=15625;
    }
    TCNT1=0;
    
  }
  
  if(PINC&(1<<PC4))
  {
    reset();//restartlama fonksiyonu
    cli();
    TCNT1=0;
    sei();
  }

}
void reset()
{
  durum=0;
  sayac=1;
  PORTB=B000000;
  OCR1A=11718.75;
  TCNT1=0;
}
ISR(TIMER1_COMPA_vect)//her bekleme süresinde bir buraya girer
{
   if(durum==1)
   {//eğer start aktif ise
    switch(sayac)
    {//ledler binary şekilde sıralı yanar
      case 1:
        PORTB=B000100;//ilk başta 10. pindeki led yanar
        sayac++;
        break;
      case 2:// sayının binary karşılığına gelen ledler sırayla yakılır   
        PORTB=B001000;
        sayac++;
        break;
      case 3:
        PORTB=B001100;
        sayac++;
        break;
      case 4:
        PORTB=B010000;
        sayac++;
        break;
      case 5:
        PORTB=B010100;
        sayac++;
        break;
      case 6:
        PORTB=B011000;
        sayac++;
        break;
      case 7:
        PORTB=B011100;
        sayac++;
        break;
      case 8:
        PORTB=B100000;
        sayac++;
        break;
      case 9:
        PORTB=B100100;
        sayac++;
        break;
      case 10:
        PORTB=B101000;
        sayac++;
        break;
      case 11:
        PORTB=B101100;
        sayac++;
        break;
      case 12:
        PORTB=B110000;
        sayac++;
        break;
      case 13:
        PORTB=B110100;
        sayac++;
        break;
      case 14:
        PORTB=B111000;
        sayac++;
        break;
      case 15:
        PORTB=B111100;
        sayac++;
        break;
    }
    if(sayac==16)
    {//eğer sayac 16 olursa 
      //sayac sıfırlanır
      sayac=1;
    }
   }
   
   if(durum==0)
   {
    //eğer durum start aktif değilse ve stop butonun basılmışsa
    //buraya girer ve hiçbir şey yapmaz
   }
}

