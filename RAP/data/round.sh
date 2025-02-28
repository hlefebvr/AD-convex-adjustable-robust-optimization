for FILE in src/*
do
  DEST=rounded/$(basename $FILE)
  awk 'NR==1 {print; next} {
      # Round all numbers to 2 decimal places
      for(i=1; i<=NF; i++) {
          $i = sprintf("%.2f", $i)
      }
      print
  }' $FILE > $DEST
done