#ifndef __MEMORIAS_H_
#define __MEMORIAS_H_

void escrita_EEPROM_interna(char endereco, char dados);

char ler_EEPROM_interna(char endereco);

void calcular_palavra_magica(void);

unsigned char verificar_checksum(void);

void update_EEPROM_interna_relogio_alarme (void);

void ler_EEPROM_interna_relogio_alarme (void);

void update_EEPROM_interna_temp_alarme (void);

void ler_EEPROM_interna_temp_alarme (void);

void update_EEPROM_interna_lum_alarme (void);

void ler_EEPROM_interna_lum_alarme (void);

void update_EEPROM_interna_relogio_hours (void);

void update_EEPROM_interna_relogio_minutes (void);

void ler_EEPROM_interna_relogio (void);

void ler_EEPROM_interna_relogio_seconds (void);

void update_EEPROM_interna_parametros (void);

void ler_EEPROM_interna_parametros (void);

void init_EEPROM_externa(void);

void writeEEPROMexterna (char endereco, char data[8]);

char readEEPROMexterna (char endereco);

void update_EEPROM_external(char codigoev);

char ler_registo(void);

#endif
