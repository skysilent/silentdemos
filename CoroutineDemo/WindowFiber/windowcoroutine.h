/* Copyright 2018
 �ر�˵�����˴������ϸ�ο��Ʒ�Ŀ�coroutine(linux ����),����Fiber
 github ��ַ��https://github.com/cloudwu/coroutine
 ��֪���᲻�����ڴ���������,���������һ���˳̵��ô�
 ��������:
    https://docs.microsoft.com/en-us/windows/desktop/api/WinBase/nf-winbase-switchtofiber 
    windows�����и�co_yield�������Ǹ��ؼ���
 ��һ�Σ������߳�תΪ�˳̣����˳�תΪ���̣߳���һ��û������������������ָ�벻֪������ȥ�ˣ�
    https://docs.microsoft.com/zh-cn/windows/desktop/api/winbase/nf-winbase-convertthreadtofiber
    https://docs.microsoft.com/zh-cn/windows/desktop/api/winbase/nf-winbase-convertfibertothread
 */

#ifndef WINDOWCOROUTINE_H_
#define WINDOWCOROUTINE_H_
//Э�̵�״̬
enum CoroutineStatus
{
    CoroutineStatus_Dead,
    CoroutineStatus_Ready,
    CoroutineStatus_Running,
    CoroutineStatus_Suspend
};
struct schedule;

/*
* ���������˳̵�����, ��ͬ���˳̴�����Ҫִ�еĺ�����.
* ���������� �������صĽ��
* �����û��Զ�������
*/
typedef void(*coroutine_function)(void* schedule, void * ud);

/*
* �����˳�ϵͳ, ���������˳�
* ���ؿ������˳̵���ϵͳ������
*/
extern schedule* coroutine_start(void);

/*
* �رտ������˳�ϵͳ
* S ��co_start ���ص��˳̹�����
*/
extern void coroutine_close(schedule*S);
/*
* ����һ���˳̶���,�����ش����˳̵�id. �����õ��˳�״̬��CS_Ready
* S �� co_start ���ص��˳̹�����
* func ���˳����е�����
* ud  �� �û����������, co_f �� ud ��ʹ��
*  ���ش����õ��˳̱�ʶid(idָ���ǵ�ǰ�Ĺ������е����λ��)
*/
extern int coroutine_create(schedule* S, coroutine_function func, void * ud);

/*
* ��������˳̶���.
* S ��Ϊ�˳̹���������
* id  ָ����coroutine_create �������˳̶���
* ������׼��״̬�͹���״̬���˳̶��󼤻
*/
extern void coroutine_resume(schedule* S, int id);

/*
* �жϵ�ǰ���еĵ��˳�, ����CPU�������˳̴������.
* S    : �˳̹���������
*/
extern void   coroutine_yield(schedule*S);

/*
* �õ���ǰ�˳����е�״̬
* S    : �˳̹���������
* id ���� coroutine_create �������˳̶��󣬾����˳��ڹ������е�λ��
*����״̬
*/
extern CoroutineStatus  coroutine_status(schedule*S, int id);

/*
* �õ���ǰ�˳�ϵͳ�����е��˳�, ���� < 0��ʾû���˳�������
* S�˳̹���������
* ���ص�ǰ���е��˳�id,
*/
extern int coroutine_running(schedule*S);


#endif // WINDOWCOROUTINE_H_

