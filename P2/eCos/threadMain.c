#include <cyg/kernel/kapi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//Processos
cyg_thread        thread_s[4];
char              stack[4][4096];
//ver bem stacks (página 11-slide eCos exemplo workers)
cyg_handle_t process, interf, send, rec;
//Caixas de Correio
cyg_handle_t pcsendpic, picsendpc, comint ;
cyg_mbox pcsendpic, picsendpc, comint;

void cyg_user_start(void)
{

  //ver prioridades (0-mais priotário, 31-menos prioritário)
  //ver tamanho da pilha
  //substituir simple_program pela rotina que trata da thread
      cyg_thread_create(0, simple_program, (cyg_addrword_t) 0,
                                     "Processamento", (void *) stack[0], 4096,
                                    &process, &thread_s[0]);
      cyg_thread_create(0, monitorinterface, (cyg_addrword_t) 1,
                                     "Interface", (void *) stack[1], 4096,
                                     &interf, &thread_s[1]);

     cyg_thread_create(2, simple_program, (cyg_addrword_t) 2,
                                     "Envio", (void *) stack[2], 4096,
                                     &send, &thread_s[2]);
     cyg_thread_create(2, simple_program, (cyg_addrword_t) 2,
                                    "Receber", (void *) stack[3], 4096,
                                    &rec, &thread_s[3]);
      cyg_thread_resume(process);
      cyg_thread_resume(interf);
//send e recieve só estão activos quando for necessária a comunicação entre o PC e a placa
      cyg_thread_suspend(send);
      cyg_thread_suspend(rec);

}


int main(void){

  cyg_user_start();

  while(1){
    //Verifica se existem mensagens na mailbox pcsendpic
      if(cyg_mbox_peek(pcsendpic)!=0){
        cyg_thread_resume(recep);
        //A thread send fica activa e espera pela mensagem com o cyg_mbox_get(), tratando
        //do envio da informação para a placa;
        //Será necessário alterar a prioridade da thread?
        //Delays necessários? Flags? -> Dúvidas Prof
        //cyg_thread_set_priority (cyg_handle_t send, 0);
      }
      if(cyg_mbox_peek(picsendpc)!=0){
        cyg_thread_resume(trans);
      }
      if(cyg_mbox_peek(comint)!=0){
        //cyg_mbox_peek_item
        //se for para o processamento;
        //se for para a Transmissão;


      }

  }
}
