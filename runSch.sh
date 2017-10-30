#PBS -l nodes=12:ppn=12
source /etc/bash.bashrc
mpirun main.out
