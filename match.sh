l=0
for i in assets/models/cars/*.glb; do
    echo "\"$(basename -s .glb "$i")\","
    ((l++))
done
echo $l