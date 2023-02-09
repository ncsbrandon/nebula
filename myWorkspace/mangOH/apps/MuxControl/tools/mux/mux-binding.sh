AddUserToAppBinding()
{
    config set users/$1/bindings/$2/app        $3
    config set users/root/bindings/$2/interface  $2
}

AddUserToAppBinding root mangoh_muxCtrl muxCtrlService
echo "mux service binding done!"
