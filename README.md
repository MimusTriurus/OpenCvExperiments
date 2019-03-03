# OpenCvExperiments
Эксперименты с OpenCV

Структура проекта:
  1. DepthMapFilter - эксперимент с постфильтрацией карт глубины;
  2. DisparityMapTuner - эксперимент с различными алгоритмами построения карты глубины (StereoBM, StereoSGBM, GPU StereoBM);
  3. VideoRecorder - запись видео с камеры в виде набора картинок;
  4. VideoFromImgs - получение видео файла из набора картинок;
  5. SlamExperiments - эксперимент с реализацией алгоритма SLAM средставами OpenCV и CUDA;
  6. CvMatProvider - библиотека получения синхронных откалиброванных кадров со стереокамеры.
----------------------------------------------
Сравнение алгоритмов построения карты глубины

[![1](http://img.youtube.com/vi/qlagTdBGdao/0.jpg)](http://www.youtube.com/watch?v=qlagTdBGdao&t "")

Disparity map StereoSGBM

[![2](http://img.youtube.com/vi/o0cSa1m251o/0.jpg)](http://www.youtube.com/watch?v=o0cSa1m251o&t "")

Disparity map GPU StereoBM

[![3](http://img.youtube.com/vi/WaxaD1ynTqM/0.jpg)](http://www.youtube.com/watch?v=WaxaD1ynTqM&t "")

Disparity map StereoBM

[![4](http://img.youtube.com/vi/1WHSZjMw7Zc/0.jpg)](http://www.youtube.com/watch?v=1WHSZjMw7Zc&t "")

Системные требования:
----------------------------------------------
1. Qt 5;
2. OpenCV 3 + CUDA + ximgproc;
