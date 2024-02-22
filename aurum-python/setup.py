from setuptools import setup, find_packages

setup(
    name             = 'aurum-python',
    version          = '0.0.1',
    description      = 'Python wrapper for aurum',
    author           = 'Hosang Kim',
    author_email     = 'hosang12.kim@samsung.com',
    url              = 'https://github.sec.samsung.net/tizen/aurum',
    install_requires = ['grpcio == 1.46.3', 'grpcio-tools == 1.46.3', 'opencv-python', 'scikit-image == 0.21.0', 'scipy == 1.10.1', 'numpy == 1.22.4', 'PyWavelets == 1.4.1'],
    packages         = find_packages(exclude = ['docs']),
    long_description=open('README.md').read(),
    keywords         = ['aurum'],
    python_requires  = '>=3.8',
    package_data={}
)