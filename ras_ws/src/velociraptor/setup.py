from setuptools import find_packages, setup

package_name = 'velociraptor'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Valeria y Maria Alejandra',
    maintainer_email='valeriaparrag@javeriana.edu.co y mariaa-diaz@javeriana.edu.co',
    description='Carrito con WIFI',
    license='MIT License Copyright (c) 2025 Valeria Andrea Parra, María Alejandra Ortiz',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
        'wifi_transmitter = velociraptor.wifi_transmitter:main',
        ],
    },
)
