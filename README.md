# PathPlanningProject

### Linux и Mac
Release сборка:
```bash
cd PathPlanningProject
cd Build
cd Release
cmake ../../ -DCMAKE_BUILD_TYPE="Release"
make
make install
```

Debug сборка:
```bash
cd PathPlanningProject
cd Build
cd Debug
cmake ../../ -DCMAKE_BUILD_TYPE="Debug"
make
make install
```

Запуск:
```bash
cd ../../Bin/{Debug|Release}/
./Alexander_Novichkov_ASearch ../../Examples/example.xml
```

### Windows
Release сборка:
```cmd
cd PathPlanningProject
cd Build
cd Release
set PATH
cmake ../../ -DCMAKE_BUILD_TYPE="Release" -G "MinGW Makefiles"
mingw32-make
mingw32-make install
```

Debug сборка:
```cmd
cd PathPlanningProject
cd Build
cd Debug
set PATH
cmake ../../ -DCMAKE_BUILD_TYPE="Debug" -G "MinGW Makefiles"
mingw32-make
mingw32-make install
```

Запуск:
```cmd
cd ../../Bin/{Debug|Release}/
Alexander_Novichkov_ASearch.exe ../../Examples/example.xml
```

### Формат входных данных
На вход поступает XML файл в качестве аргумента командной строки. 

### Формат выходных данных
Генерируется выходной XML файл.

### Формат входного XML файла
* root - корневой тег.
    * map - содержит описание карты.
        * width, height - ширина и высота карты. 
        * cellsize - размер одной ячейки.
        * startx, starty - стартовая точка.
        * finishx, finishy - конечная точка.
        * grid - таблица, описывающая карту. Номер столбца соответствует x координате, номер строки соответствует y координате. "0" - свободная клетка, "1" - препятствие.
    * algorithm
        * searchtype - алгоритм поиска.
        * metrictype -  эвристика для оценки расстояния. Поддерживаются "euclidean", "manhattan", "chebyshev", "diagonal".
        * hweight - вес эвристики при подсчете f вершины
        * allowdiagonal - разрешено ли ходить по диагонали (true/false). Если false, то ходить можно только по горизонтали/вертикали.
        * cutcorners - разрешено ли ходить по диагонали, если рядом препятствие (true/false).
        * allowsqueeze - разрешено ли ходить по диагонали, если с обеих сторон препятствия (true/false).
    * options
        * loglevel - изменяет количество выводимой информации. Поддерживаются значения "0.5" и "1".
        * logpath - путь до папки, в которой будет сгенерирован выходной XML файл.
        * logfilename - название выходного файла.

Пример входного файла: [example.xml](Examples/example.xml).
    
 ### Формат выходного XML файла
Содержит все то, что есть во входном файле и дополнительно тег log с информацией о работе алгортма.
* root
    * log
        * summary - содержит информацию о количестве итераций алгоритма, количестве созданных вершин, длине пути, времени работы алгоритма.
        * path - аналогично grig, но теперь зведзочками отмечен путь от старта до финиша.
        * lplevel - содержит коодинаты клеток из найденного пути.
        * hplevel - получается из lplevel объединением подряд идущих клеток, находящихся на одной вертикали/горизонтали.

Пример выходного файла: [example_log.xml](Examples/example_log.xml).

### Подробнее про опции алгоритма

#### Эвристики
Пусть dx, dy равны модулю разности координат между текущей позицией и финишем, a - длина стороны клетки карты, d - длина диагонали.
Тогда эвристики выражаются так:
* Euclidean: a * sqrt(dx * dx + dy * dy)
* Manhattan: a * (dx + dy)
* Chebyshev: max(dx, dy)
* Diagonal: a * |dx – dy| + d * min(dx, dy)


