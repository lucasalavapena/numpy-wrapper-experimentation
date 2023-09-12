from distutils.core import setup, Extension

def main():
    setup(name="numpywrapper",
          version="1.0.0",
          description="Numpy wrapper test",
          author="batman",
          packages=['numpywrapper'],
          author_email="your_email@gmail.com",
            ext_modules=[Extension("buffer_exposer", ["numpywrapper/buffer_exposer.c"])])


if __name__ == "__main__":
    main()

