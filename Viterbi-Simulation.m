    pe=zeros(length(SNR),1);
    error=zeros(length(SNR),1);
    isi_seq = zeros(1,num);
    z = zeros(1,num);
    mem = [-3, -1, 0]; %isi memory
    
    for s = 1:length(SNR)
        snr = db2pow(SNR(s));
        sigma = sqrt(1/(snr));
        n = normrnd(0,sigma,1,num);
        tx_seq = unidrnd(2,1,num)-1;
        for k=1:num    %generating ISI sequence
            if tx_seq(k)==1
                multiplier = 1;
            else
                multiplier = -1;
            end
            mem(1) = mem(1) + 1*multiplier;
            mem(2) = mem(2) + 2*multiplier;
            mem(3) = mem(3) + 1*multiplier;

            isi_seq(k) = mem(1);
            mem(1) = mem(2);
            mem(2) = mem(3);
            mem(3) = 0;
        end
        z = isi_seq + n; %received sequence
    
        cell = zeros(4,2);
        edge = ones(4)*Inf;
        temp = ones(4)*Inf;
    
        edge(1,1) = -4; edge(1,2) = -2;
        edge(2,3) = 0; edge(2,4) = 2;
        edge(3,1) = -2; edge(3,2) = 0;
        edge(4,3) = 2; edge(4,4) = 4;

        message = [0,0;0,1;1,0;1,1];
        
        for k=1:num
            next_msg = zeros(4,k);
            if (k == 1)
                cell(1,2) = cell(1,1) + power(z(k) + 4, 2);
                cell(2,2) = cell(1,1) + power(z(k) + 2, 2);
            elseif (k == 2)
                cell(1,2) = cell(1,1) + power(z(k) + 4, 2) ;
                cell(2,2) = cell(1,1) + power(z(k) + 2, 2);
                cell(3,2) = cell(2,1) + power(z(k), 2);
                cell(4,2) = cell(2,1) + power(z(k) - 2, 2);
            else
			for i=1:4
				for j=1:4
					if (edge(i,j) ~= Inf)
						temp(i,j) = power(z(k) - edge(i,j), 2);
                    end
                end
            end
			for i=1:4
				if (temp(floor((i-1)/2)+1,i) + cell(floor((i-1)/2)+1,1)) < (temp(floor((i-1)/2)+1 + 2,i) + cell(floor((i-1)/2)+1 + 2,1))
					cell(i,2) = cell(floor((i-1)/2)+1,1) + temp(floor((i-1)/2)+1,i);
                    next_msg(i,:) = [message(floor((i-1)/2)+1,:),mod(i-1,2)];
                else
					cell(i,2) = cell(floor((i-1)/2)+1 + 2,1) + temp(floor((i-1)/2)+1 + 2,i);
                    next_msg(i,:) = [message(floor((i-1)/2)+3,:),mod(i-1,2)];
                end
            end
            
            for i=1:4
                cell(i,1) = cell(i,2);
                cell(i,2) = Inf;    
            end
            if(k>2)
                message = next_msg;
            end
            end
        end
        [~,index] = min(cell);
        error(s) = length(find(abs(tx_seq-message(index(1),:))==1));
    end
    pe = error/num;
    semilogy(SNR,pe);
