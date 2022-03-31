# Integração da Plataforma de desenvolvimento STM32F769IDISCO com o CI ADS1198
Autores: Fábio Itturriet e Patrick Morás  
Data: 30/03/2022 
Versão: STM32Cube x.xx; TouchGFX x.xx  


_________________________________________________________________________________________________________________________________________________________________________________

# Descrição dos pinos do ADS1198 conectados a plataforma

| Pino do ads1198 | Função (lado do ADS1198) | Descrição | Pino correspondente disco-F769NI |
| --- | --- | --- | --- |
| CLKSEL | entrada digital | Seleciona o fornecimento de clock principal (interno ou externo) | PA_12 (D13) |
| DRDY#** | saída digital | Dados prontos, ativo baixo, depende do modo usado (RDATAC ou RDATA) | PB_14 (D12) |
| GPIO4 | saída/entrada digital | Pino de entrada / saída de uso geral configuráveis através de registradores; saida: registrador GPIOD define o valor da saída; entrada: multiplexado com o sinal de RESP_PH* | PB_15 (D11) |
| GPIO3 | saída/entrada digital | Pino de entrada / saída de uso geral configuráveis através de registradores; saida: registrador GPIOD define o valor da saída; entrada: multiplexado com o sinal RESP* | PA_11 (D10) |
| GPIO2 | saída/entrada digital | Pino de entrada / saída de uso geral configuráveis através de registradores saida: registrador GPIOD define o valor da saída; entrada: multiplexado com sinal RESP_BLK* | PH_6 (D9) |
| DOUT | saída digital | Saída de dados SPI, funciona juntamente com o SCLK, quando em modo contínuo DOUT também indica quando novos dados estão disponíveis (Minimizando o uso do DRDY?!?!) | PF_8 (A4) SPI5_MISO |
| GPIO1 | saída/entrada digital | Pino de entrada / saída de uso geral configuráveis através de registradores saida: registrador GPIOD define o valor da saída; entrada: multiplexado com o sinal PACEIN* | PJ_4 (D8) |
| DAISY_IN | Entrada digital | Utilizado quando se quer frontends funcionando em cadeia; curto com o DGND caso não utilizado | PC_8 (D5) |
| SCLK | Entrada digital | Clock SPI* | PF_7 (D6) SPI5_SCLK |
| CS# | Entrada digital | Seleção de chip para comunicação SPI,  ativo baixo durante toda a comunicação (no nosso sempre sera baixo), após o fim da comunicação esperar pelo menos 8 ciclos de TCLK para subir o nível lógico, quando ocorrer a borda de subida a interface serial é reiniciada | PF_6 (D3) SPI5_SSEL |
| START | Entrada digital | Iniciar conversão, ativo alto por pelo menos dois TCLK, ou o comando START enviado para iniciar a conversão, sem o comando e o START em nível lógico baixo DRDY não emite sinal (conversões são interrompidas), existem dois modos de conversão, modo continuo ou modo de disparo único. | PF_10 (A3) |
| RESET# | Entrada digital | Reinicia o sistema, leva 18 ciclos para reiniciar (podendo voltar para o sinal alto), para reiniciar pode-se utilizar o comando RESET (entra em vigor na oitava borda descendente do SCLK) | PC_2 (A2) |
| PWDN# | Entrada digital | Desliga todos os circuitos do chip, é recomendado que durante o desligamento o relógio externo tbm seja desligado para economizar energia, | PA_4 (A1) |
| DIN | Entrada digital | Entrada de dados SPI* | PF_9 (A5) SPI5_MOSI |
