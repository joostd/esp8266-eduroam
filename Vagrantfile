Vagrant.configure(2) do |config|

  config.vm.box = "ubuntu/trusty64"

  config.vm.provision "shell", privileged: false, inline: <<-SHELL
    sudo apt-get update
    sudo apt-get install -y build-essential git make unrar-free unzip autoconf automake libtool gcc g++ gperf flex bison texinfo gawk ncurses-dev libexpat-dev python sed libreadline-dev libffi-dev pkg-config
    git clone --recursive https://github.com/pfalcon/esp-open-sdk.git
  SHELL

  config.vm.provider "virtualbox" do |v|
    v.memory = 1024
  end

end
