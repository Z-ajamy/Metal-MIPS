#include"engine.h"
#include <stdlib.h>

state_t engine_init(engine_t *eng, uint32_t mem_size)
{
    uint32_t i;

    if (!eng || mem_size == 0)
        return SIM_ERR_MEM_OUT_OF_BOUNDS;

    /*the cpu init*/
    eng->cpu.pc = 0;
    for (i = 0; i < 32; i++)
    {
        eng->cpu.g_registers[i] = 0;
    }
    eng->memory.mem = calloc(1, mem_size);
	if (!eng->memory.mem)
		return (SIM_ERR_MALLOC);

    eng->memory.size = mem_size;
    cpu_init();
    eng->is_running = 1;
    return (SIM_SUCCESS);

}

void engine_cleanup(engine_t *engine)
{
    if (engine && engine->memory.mem)
	{
		free(engine->memory.mem);
		engine->memory.mem = NULL;
	}
}

state_t engine_run(engine_t *eng)
{
    state_t status;

	while (eng->is_running)
	{
		status = cpu_tick(&eng->cpu, &eng->memory);
		
		if (status != SIM_SUCCESS)
		{
			eng->is_running = 0;
			if (status == SIM_HALT)
				return (SIM_SUCCESS);
			return (status);
		}
	}
	return (SIM_SUCCESS);
}
