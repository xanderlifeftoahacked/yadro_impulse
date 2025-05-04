# Тестовое задание 

## Запуск: 
```
git clone git@github.com:xanderlifeftoahacked/yadro_impulse.git
cd yadro_impulse
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
./pcclub <filename>
```

### Зависимости:
* G++
* CMake
* GTest
  
### Тесты:
* Большая часть функционала покрыта unit-тестами.
* Запуск тестов:
```
git clone git@github.com:xanderlifeftoahacked/yadro_impulse.git
cd yadro_testtask
sudo rm -rf build
mkdir build && cd build
cmake ../tests 
make
ctest --output-on-failure
```

  

### Пример работы: 
<details> 
  <summary> input.txt </summary>
  <pre><code>
3
09:00 19:00
10
08:48 1 client1
09:41 1 client1
09:48 1 client2
09:52 3 client1
09:54 2 client1 1
10:25 2 client2 2
10:58 1 client3
10:59 2 client3 3
11:30 1 client4
11:35 2 client4 2
11:45 3 client4
12:33 4 client1
12:43 4 client2
15:52 4 client4
19:00 1 client5
19:05 4 client3
19:10 1 client6
19:15 2 client6 1
19:20 4 client6
  </pre></code>
  </details>

<details>
<summary> stdout </summary>
  <pre><code>
09:00
08:48 1 client1
08:48 13 NotOpenYet
09:41 1 client1
09:48 1 client2
09:52 3 client1
09:52 13 ICanWaitNoLonger!
09:54 2 client1 1
10:25 2 client2 2
10:58 1 client3
10:59 2 client3 3
11:30 1 client4
11:35 2 client4 2
11:35 13 PlaceIsBusy
11:45 3 client4
12:33 4 client1
12:33 12 client4 1
12:43 4 client2
15:52 4 client4
19:00 11 client3
19:00 1 client5
19:00 13 NotOpenYet
19:05 4 client3
19:05 13 ClientUnknown
19:10 1 client6
19:10 13 NotOpenYet
19:15 2 client6 1
19:15 13 ClientUnknown
19:20 4 client6
19:20 13 ClientUnknown
19:00
1 70 05:58
2 30 02:18
3 90 08:01
</pre></code>
</details>

