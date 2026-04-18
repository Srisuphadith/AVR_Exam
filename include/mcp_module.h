#ifndef MCP_MODULE_H
#define MCP_MODULE_H

#include <avr/io.h>
#include <avr/interrupt.h>

void mcp_begin();
uint16_t read_mcp();

#endif