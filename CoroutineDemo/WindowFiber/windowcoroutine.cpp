#include"windowcoroutine.h"
#include<Windows.h>
// �˳�ջ��С
#define _INT_STACK        (1024 * 1024)
// Ĭ�ϳ�ʼ�������˳���Ŀ
#define _INT_COROUTINE    (16)

/*
* �����˳̵�Ԫ coroutine , �����˳̼������� S
*/
// �˳�ջ��С 1M
#define _INT_STACK        (1024 * 1024)
// Ĭ�ϳ�ʼ�������˳���Ŀ
#define _INT_COROUTINE    (16)

/*
* �����˳̵�Ԫ coroutine , �����˳̼������� S
*/
struct coroutine;

struct schedule {
    PVOID main;                 // �˳̹������б������ʱ�˳̶���
    int running;                // ��ǰ�˳̹����������е��˳�id

    int cur_index;              // ��ǰ�˳̼���ѯ�е�ǰ����
    int cap;                    // �˳̼�����,
    int coroutine_count;        // Э�̸���
    struct coroutine ** co;     // ������˳̼�
};

struct coroutine {
    PVOID ctx;                    // ����ϵͳ�˳̶���                
    coroutine_function func;      // �˳�ִ�еĺ�����
    void * ud;                    // �˳�ִ�еĶ������
    CoroutineStatus status;       // ��ǰ�˳�����״̬
};
/* ͨ�������߳�ת��Ϊ�˳̣��˴���Ҫ���˳̹رյ�ʱ��ʹ��(���˳�תΪ�߳�)
*
*/
schedule* coroutine_start(void)
{
    struct schedule*comanag = (schedule*)malloc(sizeof(struct schedule));
    comanag->cur_index = 0;
    comanag->cap = _INT_COROUTINE;
    comanag->running = -1;
    struct coroutine **co = (coroutine**)calloc(comanag->cap, sizeof(struct coroutine*));
    comanag->co = co;
    comanag->coroutine_count = 0;
    comanag->main = ConvertThreadToFiberEx(NULL, FIBER_FLAG_FLOAT_SWITCH);
    return comanag;
}
inline void _delete_coroutine(coroutine *co) {
    DeleteFiber(co->ctx);
    free(co);
}
inline struct coroutine* _new_coroutine(coroutine_function func, void * ud) {
    struct coroutine *co = (coroutine*)malloc(sizeof(struct coroutine));
    co->func = func;
    co->ud = ud;
    co->status = CoroutineStatus_Ready;
    return co;
}

void coroutine_close(schedule* S)
{
    for (int i = 0; i < S->cap; i++) {
        struct coroutine *co = S->co[i];
        if (co) {
            _delete_coroutine(co);
            S->co[i] = NULL;
        }
    }
    free(S->co);
    S->co = NULL;
    free(S);
    S = NULL;
    ConvertFiberToThread();
}

int coroutine_create(schedule* S, coroutine_function func, void * ud)
{
    struct coroutine *co = _new_coroutine(func, ud);
    int index = S->cur_index;
    int count = S->cap;
    if (S->cur_index < S->cap) {
        for (int i = 0; i < S->cap; i++) {
            int id = (i + S->coroutine_count) % S->cap;
            if (NULL == S->co[id]) {
                S->co[id] = co;
                ++S->coroutine_count;
                ++S->cur_index;
                return id;
            }
        }
    }
    coroutine ** point = (coroutine **)realloc(S->co, sizeof(struct coroutine *) * S->cap * 2);
    memset(point + S->cap, 0, sizeof(struct coroutine *) * S->cap);
    point[count] = co;
    S->cap *= 2;
    S->co = point;
    S->coroutine_count++;
    return S->cur_index++;
}
static inline VOID WINAPI _comain(LPVOID ptr) {
    struct schedule * S = reinterpret_cast<struct schedule*>(ptr);
    int id = S->running;
    struct coroutine * co = S->co[id];

    co->func(S, co->ud);
    co = S->co[id];
    co->status = CoroutineStatus_Dead;

    SwitchToFiber(S->main);
}
/*
*/
void coroutine_resume(schedule* S, int id)
{
    if (S->running != -1) {
        int runningid = S->running;
        coroutine * co = S->co[runningid];
        S->co[runningid] = NULL;
        --S->coroutine_count;
        S->cur_index = runningid;
        S->running = -1;
        _delete_coroutine(co);
        if (runningid == id) {
            return;
        }
    }
    else {
        coroutine * co = S->co[id];
        switch (co->status)
        {
        case CoroutineStatus_Ready:
            co->ctx = CreateFiberEx(_INT_STACK, 0, FIBER_FLAG_FLOAT_SWITCH, _comain, S);
        case CoroutineStatus_Suspend:
            co->status = CoroutineStatus_Running;
            S->running = id;
            S->main = GetCurrentFiber();
            SwitchToFiber(co->ctx);
        default:
            break;
        }
    }
}

void coroutine_yield(schedule* S)
{
    if (S) {
        int id = S->running;
        struct coroutine* co = S->co[id];

        co->status = CoroutineStatus_Suspend;
        S->running = -1;
        co->ctx = GetCurrentFiber();

        SwitchToFiber(S->main);
    }
}

CoroutineStatus coroutine_status(schedule* S, int id)
{
    if (S&&id >= 0 && id<S->cap) {
        if (S->co[id]) {
            return S->co[id]->status;
        }
        return CoroutineStatus_Dead;
    }
    return CoroutineStatus_Dead;
}

int coroutine_running(schedule* S)
{
    if (S) {
        return S->running;
    }
    return -1;
}
