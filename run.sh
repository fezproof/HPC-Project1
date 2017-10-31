#PBS -l nodes=8:ppn=4
source /etc/bash.bashrc
cd $PBS_O_WORKDIR
mpirun main -l s -P 0.6 -p 2 -n 1 -M 64
