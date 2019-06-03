# LASIN
Code and data for AAAI17 paper _Combining Logical Abduction and Statistical INduction_.

# Usage

## Prequesities

- [mlpack](https://www.mlpack.org/)
- [swi-prolog](http://www.swi-prolog.org/)
- [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/)

## Compile

Make sure `pkg-config` can load `mlpack` and `swipl` headers and libraries. 

- If you have compiled [swipl-devel](https://github.com/SWI-Prolog/swipl-devel)
  from scratch and installed it in `/usr/local`, then you should use following
  command to add it to `PKG_CONFIG_PATH`:
  
  ``` shell
  export PKG_CONFIG_PATH=/usr/local/share/pkgconfig:$PKG_CONFIG_PATH
  ```
  
- The [mlpack](https://github.com/mlpack/mlpack/) library is usually installed
  in `/usr/local`, but its package config file is usually located in `lib64/pkgconfig`:
  
  ``` shell
  export PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig:$PKG_CONFIG_PATH
  ```

Then you can test the installation by:

``` shell
pkg-config --cflags mlpack
pkg-config --cflags swipl
```

If they output the library paths correctly, you can enter the `src/core`
directory and make the C external predicates for SWI-Prolog:

``` shell
cd src/core
make -j$nproc
```

## Running

First add the library paths of `mlpack` and `swipl` to your `LD_LIBRARY_PATH`:

``` shell
export LD_LIBRARY_PATH=/usr/local/lib/swipl/lib/x86_64-linux:/usr/local/lib64:$LD_LIBRARY_PATH
```

Extract the dataset:

``` shell
tar -xvf data.tar.bz2
```

Then call the testing prolog scripts in `src/prolog` directory to run the
experiments:

``` shell
cd src/prolog
swipl test.pl
```

Now please call the learning predicates to run LASIN, e.g.:

``` prolog
-? test_go_M(10, Model, [200, 0.4, 0, 100], stroke).
```

More parameter settings and examples are listed in `learn_dict.pl` and
`learn_dict_new.pl`.

## Remark

We are optimising and tidying up the code, instructions about configuration
and parameter settings will be added to this README file after the cleanup.
Sorry about the inconvenience!

# Reference
- W.-Z. Dai and Z.-H. Zhou. Combining logic abduction and statistical induction: Discovering written primitives with human knowledge. In: Proceedings of the 31st AAAI Conference on Artificial Intelligence (AAAI'17), San Francisco, CA, 2017, pp.4392-4398.
