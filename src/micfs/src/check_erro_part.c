
            {
                fl_tp Xk_chg=del_lsi(lsi,Xk_plus,M);//del_lsi是新生成的Xk_chg
                while( Xk_chg->k > 2 && Ym->k>0)
                {
                    int lsi_chg=glsi(Xk_chg,M);
                    Xk_miu_chg=del_lsi(lsi_chg,Xk_chg,M);//Xk_miu_chg是新生成的
                    if(EXACT( Xk_miu_chg->sig) > EXACT(Xk->tail->pn->sig) )
                    {
                        lst_rplc(&Xk_chg,&Xk_miu_chg);//交换了Xk_chg和Xk_miu_chg的指向地址，
                        free(Xk_miu_chg->membs);        //释放实际内存为Xk部分的值
                        free(Xk_miu_chg);               //释放指向结构指针
                        add_node(Xk->tail->atr,Ym,Xk,M);//设置k=k-1
                    }
                    else
                    {
                        add_node(msw,Xk,Ym,M);
                        fl_tp nXk=del_lsi(lsi,Xk,M);
                        lst_rplc(&Xk,&nXk);
                        free(nXk->membs);
                        free(nXk);
                        free(Xk_miu_chg->membs);
                        free(Xk_miu_chg);
                        break;
                    }
                }
                free(Xk_plus->membs);
                free(Xk_plus);
            }
