#PBS -l walltime=0:10:00,nodes=2:ppn=4
#PBS -e ./error.txt
#PBS -o ./output.csv
source /etc/bash.bashrc
cd $PBS_O_WORKDIR
mpirun main -l s -P 0.8 -p 2 -n 1 -N 8 -t 1 -T 6 -s 4096 -S 4096
