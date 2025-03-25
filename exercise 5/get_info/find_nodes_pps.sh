
#stname of the master node
echo "Master node: $(hostname)"

# Check for PBS or SLURM environment variables
if [ -n "$PBS_NODEFILE" ]; then
    echo "Using PBS scheduler..."
    echo "Nodes assigned:"
    sort $PBS_NODEFILE | uniq -c

    echo "Total number of processors:"
    wc -l < $PBS_NODEFILE

elif [ -n "$SLURM_NODELIST" ]; then
    echo "Using Slurm scheduler..."
    echo "Nodes assigned:"
    scontrol show hostname $SLURM_NODELIST

    echo "Processors per node:"
    echo $SLURM_CPUS_ON_NODE

    echo "Total number of nodes:"
    scontrol show hostname $SLURM_NODELIST | wc -l
else
    echo "No scheduler detected. Running MPI on the current machine only."
    echo "Node: $(hostname)"
    echo "Processors: $(nproc)"
fi


