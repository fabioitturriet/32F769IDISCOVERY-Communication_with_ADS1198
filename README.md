# Integração da Plataforma de desenvolvimento STM32F769IDISCO com o CI ADS1198
Autores: Fábio Itturriet e Patrick Morás  
Data: 30/03/2022 
Versão: STM32Cube x.xx; TouchGFX x.xx  


_________________________________________________________________________________________________________________________________________________________________________________

# Descrição dos pinos do ADS1198 conectados a plataforma

| Pino do ads1198 | Função (lado do ADS1198) | Descrição | Pino correspondente disco-F769NI |
| --- | --- | --- | --- |
| CLKSEL | entrada digital | Seleciona o fornecimento de clock principal (interno ou externo) | PA_12 (D13) |
| DRDY# | saída digital | Dados prontos, ativo baixo, depende do modo usado (RDATAC ou RDATA) | PB_14 (D12) |
| GPIO4 | saída/entrada digital | Pino de entrada / saída de uso geral configuráveis através de registradores; saida: registrador GPIOD define o valor da saída; entrada: multiplexado com o sinal de RESP_PH* (nao Configurado)| PB_15 (D11) |
| GPIO3 | saída/entrada digital | Pino de entrada / saída de uso geral configuráveis através de registradores; saida: registrador GPIOD define o valor da saída; entrada: multiplexado com o sinal RESP* (nao configurado) | PA_11 (D10) |
| GPIO2 | saída/entrada digital | Pino de entrada / saída de uso geral configuráveis através de registradores saida: registrador GPIOD define o valor da saída; entrada: multiplexado com sinal RESP_BLK* (nao configurado) | PH_6 (D9) |
| DOUT | saída digital | Saída de dados SPI, funciona juntamente com o SCLK | PF_8 (A4) SPI5_MISO |
| GPIO1 | saída/entrada digital | Pino de entrada / saída de uso geral configuráveis através de registradores saida: registrador GPIOD define o valor da saída; entrada: multiplexado com o sinal PACEIN* (nao configurado)| PJ_4 (D8) |
| DAISY_IN | Entrada digital | Utilizado quando se quer frontends funcionando em cadeia; curto com o DGND caso não utilizado | PC_8 (D5) |
| SCLK | Entrada digital | Clock SPI* | PF_7 (D6) SPI5_SCLK |
| CS# | Entrada digital | Seleção de chip para comunicação SPI,  ativo baixo durante toda a comunicação, após o fim da comunicação esperar pelo menos 8 ciclos de TCLK para subir o nível lógico, quando ocorrer a borda de subida a interface serial é reiniciada | PF_6 (D3) |
| START | Entrada digital | Iniciar conversão, ativo alto por pelo menos dois TCLK, ou o comando START enviado para iniciar a conversão, sem o comando e o START em nível lógico baixo DRDY não emite sinal (conversões são interrompidas). utilizando o comando, o pino Start deve ficar em nivel lógico baixo (utilizado dessa maneira por enquanto). | PF_10 (A3) |
| RESET# | Entrada digital | Reinicia o sistema, leva 18 ciclos para reiniciar (podendo voltar para o sinal alto), para reiniciar pode-se utilizar o comando RESET (entra em vigor na oitava borda descendente do SCLK) (utilizado o comando por enquanto) | PC_2 (A2) |
| PWDN# | Entrada digital | Desliga todos os circuitos do chip, é recomendado que durante o desligamento o relógio externo tbm seja desligado para economizar energia, | PA_4 (A1) |
| DIN | Entrada digital | Entrada de dados SPI* | PF_9 (A5) SPI5_MOSI |


# Configurações

Foi utilizado o SPI5 para comunicação com o ADS, o clock fornecido para SPI5 é pelo barramento APB2 que pode ter 108 MHz (max).

<img src="https://user-images.githubusercontent.com/86391684/161346022-37294d5f-9cf5-443d-91d5-6c383566594c.png" width="800" />

O mínimo periodo para SCLK é 50ns, ou seja é um maximo de 20MHz de frequência, inicialmente foi utilizado o prescaler do SPI5 para diminuir a frequência 12.5MHz. No manual do ADS1198 também vemos outra configurações que devem ser feitas: NOTE: SPI settings are CPOL = 0 and CPHA = 1.

<img src="https://user-images.githubusercontent.com/86391684/161348111-40264f61-53f0-44c4-904e-b83d081161cf.png" width="650" />

# Alimentação

Conforme o esquematico realizado a alimentação do CI segue os valores tipicos de alimentação dada na tabela do datasheet. A alimentação digital (DVDD) tem 1.8V e a alimentação analógica (AVDD) tem 3.3V. Na tabela também notamos que a tensão máxima de entrada digital no CI é de DVDD+0.1 V ou seja 1.9V e a mínima é de 1.44V, ja que a tensao digital disponível nas plataforma de desenvolvimento é 3.3V foi realizado um divisor de tensão reduzindo-a para 1.65V.

<img src="https://user-images.githubusercontent.com/86391684/162265828-cbb01b2c-2d9b-44ba-9ea8-d5bffacf4442.png" width="700" />


# Atualizações

conferir circuito DRL

discutir sobre circuito externo para contador de batimentos cardiacos



DGND e AGND deve ser juntado na PCB

# Recursos avançados de depuração: SWV

https://www.youtube.com/watch?v=Zqwq9nzTNF8
