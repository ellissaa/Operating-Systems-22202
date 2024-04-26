#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <stdio.h>
#include <unistd.h>

void tracee(int argc, char** argv) {
    ptrace(PTRACE_TRACEME, 0, NULL, NULL); // Указываем, что родительский процесс будет отслеживать
    execvp(argv[0], argv); // Исполняем указанный файл
}

void tracer(pid_t pid) {
    int status = 0;
    waitpid(pid, &status, 0); // Дожидаемся запроса на отслеживание от дочернего процесса
    ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD); // Настройка для того, чтобы различать системные вызовы и другие остановки
    
    while (!WIFEXITED(status)) { // Пока дочерний процесс не завершился
        struct user_regs_struct state; // Структура для сохранения значений регистров
        ptrace(PTRACE_SYSCALL, pid, 0, 0); // Исполнять процесс до входа или выхода из сискола
        waitpid(pid, &status, 0);
        /**
         * Если дочерний процесс был остановлен сигналом (это как раз происходит при отслеживании)
         * и произошел системный вызов
         * */
        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
            ptrace(PTRACE_GETREGS, pid, 0, &state); // Читаем состояние регистров
            printf("SYSCALL %lld\n", state.orig_rax);

            ptrace(PTRACE_SYSCALL, pid, 0, 0);
            waitpid(pid, &status, 0);
        }
    }
}

// argc и argv для процесса, который отслеживаем
void trace(int argc, char** argv) {
    pid_t pid = fork(); // Порождаем новый процесс-копию текущего.
    if (pid == 0) {
        tracee(argc, argv); // Дочерний будем отслеживать
    }
    else {
        tracer(pid); // Родительский будет отслеживать
    }
}

int main(int argc, char** argv) {
    trace(argc - 1, argv + 1);
}
