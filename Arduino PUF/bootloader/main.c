#include "usart.h"
// #include "stk500.h"
#include <avr/pgmspace.h>
#include <avr/io.h>
#include "puf.h"
#include <time.h>
#include "utils_arduino.h"
#include "hkdf.h"
#include "cipher.h"

#define BAUD_RATE 115200UL
#define BAUD_SETTING ((F_CPU / (8UL * BAUD_RATE)) - 1UL)

unsigned const int __attribute__((section(".version"))) optiboot_version = 2049U;
uint8_t __attribute__((section(".noinit"))) bss_puf[PUF_SIZE];

/** Functions Prototypes */
void __init0(void) __attribute__((naked)) __attribute__((section(".init0")));
void __init1(void) __attribute__((naked)) __attribute__((section(".init1")));
void __start(void)  __attribute__((section(".startup")));



int main(void)
{
    srand(time(NULL));
    uint8_t ch = 0;
    uint8_t key[16U] = {0U};
    uint8_t id[4U] = {0U};
    id[3] = 1U;
    uint8_t prk[16U];
    // Da generare randomicamente
    uint8_t ninit[6] = {0U};
    int result;

    for (;;)
    {
        /* read a character from the console */
        ch = usart_rx_ch();

        /* IF you type 'p' you'll get the puf */
        if ('p' == ch)
        {
            usart_tx_buffer(bss_puf, PUF_SIZE);
        }
        else if (ch == 'k')
        {
            usart_tx_buffer(key, 16U);
        }
        else if (ch == 'f')
        {
            //esegui il FE
            usart_tx_string("eseguo FE\n\r");
            puf_extract(bss_puf, key);
            usart_tx_string("fine del FE\n\r");
            //usart_tx_string("prova\n\r");
        }
        else if (ch == 'i')
        {
           	usart_tx_string("ID:");
    		usart_tx_buffer(id, sizeof(id));
        }
        else if (ch == 'n')
        {
        	gen(ninit,sizeof(ninit));
    		usart_tx_string("Ninit: ");
    		usart_tx_buffer(ninit,sizeof(ninit));
    	}
    	else if (ch == 'h')
    	{
    		
    		// Calcolo PRK
    		result = hkdf(SHA256, NULL, 0, key, FE_KEY_SIZE, ninit, sizeof(ninit), prk, FE_KEY_SIZE);
    		// Check for errors
    		if (result != 0)
    		{
    		    usart_tx_string("HKDF failed with error\n");
    		    return 1;
    		}
    		usart_tx_string("PRK: ");
    		usart_tx_buffer(prk,sizeof(prk));
    		
    		//Inserisci messaggio e mac
    uint8_t ma[16] = {0U};
    uint8_t ns[6] = {0U};
    usart_tx_string("Inserisci messaggio \n");
    usart_rx_buffer(ma,sizeof(ma));
    usart_tx_string("Messaggio inserito: ");
    usart_tx_buffer(ma,sizeof(ma));

    uint8_t oldDigest[32U];
    usart_tx_string("Inserisci Mac:\n");
    usart_rx_buffer(oldDigest,sizeof(oldDigest));
    usart_tx_string("Mac inserito: ");
    usart_tx_buffer(oldDigest,sizeof(oldDigest));

    //Decifra messaggio
    Decrypt(ma, prk);
    usart_tx_string("Decripted ma: ");
    usart_tx_buffer(ma,sizeof(ma));
    //Check per ID e Ninit
    for (int i = 0; i < sizeof(id); i++)
    {
        if(ma[i]!=id[i]){
            usart_tx_string("Errore ID non corrisponde\n");
            return -1;
            }
    }
    for (int i = 0; i < sizeof(ninit); i++)
    {
        if(ma[i+sizeof(id)]!=ninit[i]){
            usart_tx_string("Errore Ninit non corrisponde\n");
            return -1;
            }
    }
    //Estraggo ns
    for (int i = 0; i < sizeof(ns); i++)
    {
        ns[i] = ma[i + sizeof(id) + sizeof(ninit)];
    }
    usart_tx_string("Ns: ");
    usart_tx_buffer(ns,sizeof(ns));
    

    // Creare un nuovo mac con gli stessi dati e compararlo (cifrare prima i dati del messaggio)
    Encrypt(ma,prk);

    //Crea mac=(ms||key||ns)
    uint8_t mac[sizeof(ma) + sizeof(key) + sizeof(ns)];
    for (int i = 0; i < sizeof(ma); i++)
    {
        mac[i] = ma[i];
    }
    for (int i = 0; i < sizeof(key); i++)
    {
        mac[i + sizeof(ma)] = key[i];
    }
    for (int i = 0; i < sizeof(ns); i++)
    {
        mac[i + sizeof(ma) + sizeof(key)] = ns[i];
    }
    usart_tx_string("Messaggio prima del mac\n");
    usart_tx_buffer(mac,sizeof(mac));

    // Lunghezza digest 32 per sha256
    uint8_t digest[32U];
    result = hmac(SHA256, mac, sizeof(mac), key, sizeof(key), digest);
    // Check for errors
    if (result != 0)
    {
        usart_tx_string("HMAC failed with error\n");
        return 1;
    }
    // digest=mac(message||k||ns)
    usart_tx_string("Digest mac\n");
    usart_tx_buffer(digest,sizeof(mac));

    // Compara mac
    result = 1;
    for (uint8_t i = 0U; (i < 32) && result; i++)
    {
        result = (digest[i] == oldDigest[i]);
    }
    if (result == 1)
    {
        usart_tx_string("Mac verificato\n");
    }
    else
    {
        usart_tx_string("Mac non autenticato\n");
    }

    //Prepara messaggio e mac
    uint8_t na[6] = {0U};
    gen(na,sizeof(na));

    // ms (id,ns,na)
    uint8_t ms[sizeof(id) + sizeof(ns) + sizeof(na)] = {0U};
    for (int i = 0; i < sizeof(id); i++)
    {
        ms[i] = id[i];
    }
    for (int i = 0; i < sizeof(ns); i++)
    {
        ms[i + sizeof(id)] = ns[i];
    }
    for (int i = 0; i < sizeof(na); i++)
    {
        ms[i + sizeof(ns) + sizeof(id)] = na[i];
    }
    Encrypt(ms,prk);
    usart_tx_string("Messaggio ms cifrato: ");
    usart_tx_buffer(ms,sizeof(ms));
    uint8_t mac3[sizeof(ms) + sizeof(na)] = {0U};
    for (int i = 0; i < sizeof(ms); i++)
    {
        mac3[i] = ms[i];
    }
    for (int i = 0; i < sizeof(na); i++)
    {
        mac3[i+sizeof(ms)] = na[i];
    }
    usart_tx_string("Messaggio prima del mac\n");
    usart_tx_buffer(mac3,sizeof(mac3));

    // Lunghezza digest 32 per sha256
    uint8_t digest3[32U];
    result = hmac(SHA256, mac3, sizeof(mac3), key, sizeof(key), digest3);
    // Check for errors
    if (result != 0)
    {
        usart_tx_string("HMAC failed with error \n");
        return 1;
    }
    // mac(message||na)
    usart_tx_string("Digest3 mac\n");
    usart_tx_buffer(digest3,sizeof(digest3));
    return 0;
    	}
        

        // qui esegui ciÃ² che fa un bootloader
        // void run_stk(void);
    }

    return 0;
}

void __init0(void)
{
    /* Reset __zero_reg__ (r1) */
    asm volatile("clr __zero_reg__");

    // setta lo stack pointer a RAMEND (0x8FF)
    asm volatile(
        "ldi r16, 0x08\n\t"
        "out __SP_H__,r16\n\t"
        "ldi r16, 0xff\n\t"
        "out __SP_L__,r16\n\t"
    );

    usart_init((unsigned int)BAUD_SETTING);

    usart_tx_string("puf init0:\n\r");
    usart_tx_buffer(bss_puf, PUF_SIZE);
}

void __init1(void)
{
    usart_tx_string("puf init1:\n\r");
    usart_tx_buffer(bss_puf, PUF_SIZE);
}

void __start(void)
{
    __asm volatile("jmp __vectors");
}
