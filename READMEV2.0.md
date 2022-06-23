# Descrição dos pinos do ADS1198 conectados a plataforma versão 2.0

| Pino do ads1198 | Função (lado do ADS1198) | Descrição | Pino correspondente disco-F769NI |
| --- | --- | --- | --- |
| CLKSEL | entrada digital | Seleciona o fornecimento de clock principal (interno ou externo) | PB_14 (D12) |
| DRDY# | saída digital | Dados prontos, ativo baixo, depende do modo usado (RDATAC ou RDATA) | PF_6 (D3) |
| GPIO4 | saída/entrada digital | Pino de entrada / saída de uso geral configuráveis através de registradores; saida: registrador GPIOD define o valor da saída; entrada: multiplexado com o sinal de RESP_PH* (nao Configurado)| PB_15 (D11) |
| GPIO3 | saída/entrada digital | Pino de entrada / saída de uso geral configuráveis através de registradores; saida: registrador GPIOD define o valor da saída; entrada: multiplexado com o sinal RESP* (nao configurado) | PA_11 (D10) |
| GPIO2 | saída/entrada digital | Pino de entrada / saída de uso geral configuráveis através de registradores saida: registrador GPIOD define o valor da saída; entrada: multiplexado com sinal RESP_BLK* (nao configurado) | PH_6 (D9) |
| DOUT | saída digital | Saída de dados SPI, funciona juntamente com o SCLK | PF_8 (A4) SPI5_MISO |
| GPIO1 | saída/entrada digital | Pino de entrada / saída de uso geral configuráveis através de registradores saida: registrador GPIOD define o valor da saída; entrada: multiplexado com o sinal PACEIN* (nao configurado)| PJ_4 (D8) |
| DAISY_IN | Entrada digital | Utilizado quando se quer frontends funcionando em cadeia; curto com o DGND caso não utilizado | PC_8 (D5) |
| SCLK | Entrada digital | Clock SPI* | PF_7 (D6) SPI5_SCLK |
| CS# | Entrada digital | Seleção de chip para comunicação SPI,  ativo baixo durante toda a comunicação, após o fim da comunicação esperar pelo menos 8 ciclos de TCLK para subir o nível lógico, quando ocorrer a borda de subida a interface serial é reiniciada | PJ_0 (D4) |
| START | Entrada digital | Iniciar conversão, ativo alto por pelo menos dois TCLK, ou o comando START enviado para iniciar a conversão, sem o comando e o START em nível lógico baixo DRDY não emite sinal (conversões são interrompidas). utilizando o comando, o pino Start deve ficar em nivel lógico baixo (utilizado dessa maneira por enquanto). | curto com DGND |
| RESET# | Entrada digital | Reinicia o sistema, leva 18 ciclos para reiniciar (podendo voltar para o sinal alto), para reiniciar pode-se utilizar o comando RESET (entra em vigor na oitava borda descendente do SCLK) (utilizado o comando por enquanto) | PC_2 (A2) |
| PWDN# | Entrada digital | Desliga todos os circuitos do chip, é recomendado que durante o desligamento o relógio externo tbm seja desligado para economizar energia, | PA_4 (A1) |
| DIN | Entrada digital | Entrada de dados SPI* | PF_9 (A5) SPI5_MOSI |



AVDD foi para 5V e DVDD foi para 3.3V, dessa maneria não são necessários divisores de tensão na interface entre o ADS e o MCU, reduzindo a complexidade do circuito.


# Clock

explicar pq o uso do oscilador externo (precisão melhor)
