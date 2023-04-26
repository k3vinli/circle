#include <circle/syscallhandler.h>
#include <circle/logger.h>
#include <circle/sched/scheduler.h>
#include <string.h>
#include <circle/timer.h>

int SyscallHandler(int arg1, int arg2, int arg3, int arg4) {
	register long syscall_no asm ("r7");
	// CLogger::Get ()->Write ("SyscallHandler", LogDebug, "syscall_no=0x%x.", syscall_no);

	// TODO: Handle system calls based on syscall_no.
	// first arg char*
	switch (syscall_no)
	{
	case 0:
	// timer.h
		return CTimer::Get()->GetTime();
		break;
	case 1:
	//scheduler get current task
	//get_task_name((char *)arg1, arg2)
		strncpy((char *) arg1, CScheduler::Get()->GetCurrentTask()->GetName(), arg2);
		break;
	case 2:
	// use logger
		CLogger::Get()->Write("SyscallHandler", LogDebug, "%s", (const char *) arg1);
	break;
	case 3:
	// call scheduler sleep
		CScheduler::Get()->Sleep(arg1);
	break;
	case 4:
	// user task calls exit, traps in kernel, should never return
	// look at task.cpp (how kernel task is implemented)
	// set state to terminated
	// do event.set
	//
	CTask *task = CScheduler::Get()->GetCurrentTask();
	task->m_State = TaskStateTerminated;
	task->m_Event.Set();
	CScheduler::Get()->Yield();
	break;
	}
	return 0;
}
