#ifndef MCP_MODULE_H
#define MCP_MODULE_H

#include <avr/io.h>
#include <avr/interrupt.h>

void mcp_module();
uint16_t mcp_read();

#endif