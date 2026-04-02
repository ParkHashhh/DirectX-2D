#include "ThreadBase.h"

CThreadBase::CThreadBase()
{
}

CThreadBase::~CThreadBase()
{
    mLoop = false;

    if (mThread)
    {
        WaitForSingleObject(mThread, INFINITE);

        // 스레드 강제 종료 함수.
        //TerminateThread(mThread, 0);

        CloseHandle(mThread);
    }
    Sleep(1);
    DeleteCriticalSection(&mCrt);
}

bool CThreadBase::Init(bool Pause)
{
    // 스레드간 데이터 공유를 위한 큐 생성
    mQueue.reset(new CThreadQueue);

    InitializeCriticalSection(&mCrt);

    unsigned int InitFlag = 0;

    if (Pause)
        InitFlag = CREATE_SUSPENDED;

    mThread = (HANDLE)_beginthreadex(nullptr, 0,
        CThreadBase::ThreadFunc, (void*)this, InitFlag, nullptr);

    if (!mThread)
        return false;

    return true;
}

void CThreadBase::Exit()
{
    mLoop = false;

    TerminateThread(mThread, 0);
}

void CThreadBase::AddData(int Header, int Size, unsigned char* Data)
{
    mQueue->push(Header, Size, Data);
}

void CThreadBase::GetData(int& Header, int& Size, unsigned char* Data)
{
    mQueue->pop(Header, Size, Data);
}

int CThreadBase::GetQueueSize()
{
    return mQueue->size();
}

bool CThreadBase::EmptyQueue()
{
    return mQueue->empty();
}

void CThreadBase::Suspend()
{
    if (mThread)
        SuspendThread(mThread);
}

void CThreadBase::Resume()
{
    if (mThread)
        ResumeThread(mThread);
}

unsigned int __stdcall CThreadBase::ThreadFunc(void* Arg)
{
    CThreadBase* Thread = (CThreadBase*)Arg;

    if (Thread)
    {
        Thread->Run();

        // 멤버 변수에 접근하기 전, 객체가 아직 유효한지 최소한의 확인이 필요합니다.
        // 하지만 객체가 delete 중이라면 이 역시 위험할 수 있습니다.
        CSync sync(&Thread->mCrt);
        Thread->mComplete = true;
    }

    return 0;
}
