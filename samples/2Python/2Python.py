import MASS

MASS.InitEngine()
t1 = MASS.LoadTextureResource("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\background.jpeg")
t2 = MASS.LoadTextureResource("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\Actor1.png")
t3 = MASS.LoadTextureResource("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\Actor2.png")
s1 = MASS.LoadSoundResource("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\bkg.mp3")
s2 = MASS.LoadSoundResource("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\chop.mp3")

a1 = MASS.GenerateActor("张三")
a2 = MASS.GenerateActor("李四")
MASS.AddTextureForActor(a1, "normal", t2)
MASS.AddTextureForActor(a2, "normal", t3)
MASS.SetActiveTextureForActor(a1, "normal")
MASS.SetActiveTextureForActor(a2, "normal")

MASS.PlayAction_UseScene(t1)
MASS.PlayAction_PlaySound(s1, 1, 1)
MASS.PlayAction_Voiceover("这是旁白这是旁白这是旁白")
MASS.PlayAction_Enter(a1, 'L')
MASS.PlayAction_Enter(a2, 'R')
MASS.PlayAction_Say_ByName("张三", "张三说话了张三说话了张三说话了")
MASS.PlayAction_Say_ByActor(a2, "李四说话了李四说话了李四说话了")
MASS.PlayAction_PlaySound(s2, 2)
MASS.PlayAction_Attack_ByActor(a1)
MASS.PlayAction_Delay(2000)
MASS.PlayAction_PlaySound(s2, 2)
MASS.PlayAction_Attack_ByPosition('R')
MASS.PlayAction_Retreat_ByActor(a1)
MASS.PlayAction_Retreat_ByPosition('R')
MASS.PlayAction_StopSound(1)

MASS.PlayAction_Voiceover("这是旁白这是旁白这是旁白")
MASS.PlayAction_Enter(a1, 'L')
MASS.PlayAction_Enter(a2, 'R')
MASS.PlayAction_Say_ByName("张三", "张三说话了张三说话了张三说话了")
MASS.PlayAction_Exit_ByActor(a1)
MASS.PlayAction_Say_ByActor(a2, "李四说话了李四说话了李四说话了")
MASS.PlayAction_Exit_ByPosition('R')
MASS.PlayAction_Finish()

MASS.FreeTextureResource(t1)
MASS.FreeTextureResource(t2)
MASS.FreeTextureResource(t3)
MASS.FreeSoundResource(s1)
MASS.FreeSoundResource(s2)
