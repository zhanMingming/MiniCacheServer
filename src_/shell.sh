function func(){
    ./$1
    echo $1
    ./$2
    echo $2
}
func $1 $2
