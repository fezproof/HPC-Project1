#PBS -l walltime=0:10:00,nodes=3:ppn=4
#PBS -m abe
#PBS -M matthew.stagoll@gmail.com
#PBS -e ./error.txt
#PBS -o ./outputB.csv
source /etc/bash.bashrc
cd $PBS_O_WORKDIR
mpirun main -l b -P 0.8 -p 2 -n 1 -N 12 -t 1 -T 6 -s 4096 -S 4096
