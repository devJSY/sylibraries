#include "MultiThread.h"
#include <bits/stdc++.h>
#include <thread>
#include <mutex> // semaphore는 없음
#include <atomic>
#include <future>
#include <random>

std::mutex mtx; // 여러 스레드가 같이 접근할수 있는 위치 이기만 하면됨

namespace sy
{
    using namespace std;

    void dotProductNaive(const vector<int>& v0, const vector<int>& v1, const unsigned int start, const unsigned int end,
                         unsigned long long& sum)
    {
        for (unsigned int i = start; i < end; ++i)
        {
            std::scoped_lock lock(mtx);
            sum += (v0[i] * v1[i]);
        }
    }

    int MultiThread()
    {
        // LambdaFunc();

        //// 논리 프로세서 갯수
        // cout << std::thread::hardware_concurrency() << endl;
        // cout << std::this_thread::get_id() << endl; // thread id

        //// 자식 스레드
        // std::thread t1 = std::thread([] { cout << std::this_thread::get_id() << endl; });
        // t1.join(); // 자식스레드가 끝날때 까지 대기

        //// 여러개의 thread 사용하기
        //{
        //    const int num_pro = std::thread::hardware_concurrency();

        //    vector<thread> my_threads;
        //    my_threads.resize(num_pro);

        //    for (auto& e : my_threads)
        //    {
        //        e = std::thread([]() {
        //            cout << this_thread::get_id() << endl;
        //            while (true){}
        //        });
        //    }

        //    for (auto& e : my_threads)
        //    {
        //        e.join();
        //    }
        //}

        //// multi-threading
        // auto work_func = [](const string& name, const int j) {
        //     for (size_t i = 0; i < 5; i++)
        //     {
        //         cout << name << " " << j << " " << this_thread::get_id() << " is working " << i << endl;
        //     }
        // };

        // std::thread t1 = std::thread(work_func, "thread 1", 123);
        // std::thread t2 = std::thread(work_func, "thread 2", 456);

        // t1.join();
        // t2.join();

        //// mutex
        // auto work_func = [](const string& name) {
        //     for (size_t i = 0; i < 5; i++)
        //     {
        //         mtx.lock(); // 다른 스레드가 들어오지못하게 lock 한뒤 작업
        //         cout << name << " " << this_thread::get_id() << " is working " << i << endl;
        //         mtx.unlock();   // 작업이 끝난뒤 unlock 필수
        //     }
        // };

        // std::thread t1 = std::thread(work_func, "thread 1");
        // std::thread t2 = std::thread(work_func, "thread 2");

        // t1.join();
        // t2.join();

        //=====================================================

        // Race Condition - 동일한 데이터를 여러 스레드들이 접근하는 과정에서 생기는 문제
        //// Race Condition 해결방법 1 - atomic
        // atomic<int> shared_memory(0);

        // auto count_func = [&]() {
        //     for (size_t i = 0; i < 1000; i++)
        //     {
        //         shared_memory++;
        //     }
        // };

        // thread t1 = thread(count_func);
        // thread t2 = thread(count_func);

        // t1.join();
        // t2.join();

        // cout << shared_memory << endl;

        // Race Condition 해결방법 2 - mutex

        // int shared_memory(0);

        // auto count_func = [&]() {
        //     for (size_t i = 0; i < 100; i++)
        //     {
        //         //// 기본적인 mutex
        //         // mtx.lock();
        //         // shared_memory++;
        //         // mtx.unlock();

        //        //// std::lock_guard 지역변수로 선언해서 {}가 종료될대 unlock 처리
        //        //std::lock_guard lock(mtx);
        //        //shared_memory++;

        //        // std::scoped_lock C++17 lock_guard 보다 진보된 기능
        //        std::scoped_lock lock(mtx);
        //        shared_memory++;
        //    }
        //};

        // thread t1 = thread(count_func);
        // thread t2 = thread(count_func);

        // t1.join();
        // t2.join();

        // cout << shared_memory << endl;

        // ===================================
        // 멀티 쓰레드 예제

        const long long numData = 1000;
        const unsigned int numThread = 4;

        vector<int> vec0, vec1;
        vec0.reserve(numThread);
        vec1.reserve(numThread);

        std::random_device seed;
        std::mt19937_64 makerand(seed());
        std::uniform_int_distribution<> range(1, 10); // 1 ~ 10 랜덤넘버

        for (long long i = 0; i < numData; ++i)
        {
            vec0.push_back(range(makerand));
            vec1.push_back(range(makerand));
        }

        cout << "실험 1번 - std::inner_product\n";
        {
            const auto sta = std::chrono::steady_clock::now(); // 시간 측정 시작
            const auto sum = std::inner_product(vec0.begin(), vec0.end(), vec1.begin(), 0ull);
            // 두 벡터의 개수가 같다고 가정하므로 vec1은 begin만 있어도 되며 ull은 unsigned long long의 약자
            const chrono::duration<double> dur = chrono::steady_clock::now() - sta;

            cout << "연산에 소요된 시간 = " << dur.count() << '\n';
            cout << "결과값 = " << sum << '\n';
            cout << '\n';
        }

        cout << "실험 2번 - Naive\n";
        {
            const auto sta = std::chrono::steady_clock::now();

            unsigned long long sum = 0;

            vector<std::thread> threads;
            threads.resize(numThread);

            const unsigned int numPerThread = numData / numThread;
            for (unsigned int t = 0; t < numThread; ++t)
                threads[t] = std::thread(dotProductNaive, std::ref(vec0), std::ref(vec1), t * numPerThread,
                                         (t + 1) * numPerThread, std::ref(sum));
            for (unsigned int t = 0; t < numThread; ++t)
                threads[t].join();

            const chrono::duration<double> dur = chrono::steady_clock::now() - sta;

            cout << "연산에 소요된 시간 = " << dur.count() << '\n';
            cout << "결과값 = " << sum << '\n';
            cout << '\n';
        }

        return 0;
    }

    void LambdaFunc()
    {
        // 함수 포인터 람다
        // auto func = [](const int& i) -> void { cout << i << endl; };
        // func(1); func(2);

        // 익명 함수 람다
        //[](const int& i) -> void { cout << i << endl; }(1234); // 1234 출력

        // lambda-introducer
        //  스코프 내의 변수들을 사용할수 있게해주는 키워드
        //{
        //     string name = "JackJack";
        //     [=]() { cout << name << endl; }();
        // }
        //  & - 밖에있는 변수를 레퍼런스로 가져옴 [&변수] 이렇게도 사용가능
        //  = - 복사
        //  this - 멤버 변수

        // vector<int> vec;
        // vec.push_back(1);
        // vec.push_back(2);

        // for_each(vec.begin(), vec.end(), [](int i) { cout << i << endl; });

        //// std function - 함수 포인터를 체계화 시킨것
        // std::function<void(int)> func = [](int i) { cout << i << endl; };
        // func(1);

        //// bind 인자가 456인 func함수를 묶어서 func2라는 함수를 정의
        // std::function<void(void)> func2 = bind(func, 456);
        // func2();

        // std::placeholders 인자 갯수 지정
        class Object
        {
        public:
            void hello(string str) { cout << str << endl; }
        };

        Object object;
        auto func = std::bind(&Object::hello, &object, std::placeholders::_1);
        func("Test Hello");

        return;
    }
} // namespace sy
