import {Injectable, Inject, Component, Directive, InjectionToken, Type, OnInit} from "@angular/core";
import {AppConfig} from "src/app/app.config";
//import {TokenStorageService} from "src/app/_services/token-storage.service";
//import {translateRememberLangComponent,/* TRANSLATION_PROVIDERS, TRANSLATIONS*/} from "src/app/pipes/remember-lang/remember-lang.component";
//import {translateRememberIangComponent} from "./translate-remember-lang/translations.component";

export const TRANSLATIONS = new InjectionToken<any>('translations', {
  providedIn: 'root',
  factory: () => sessionStorage
});
export const LANG_ENG_NAME = 'eng';
export const LANG_ENG_TRANS = {
  "Example"                                                            :   "Example",
  "must be a well-formed email address"                                :   "must be a well-formed email address",
  "Error: A registry process should be made!"                          :   "A registry process should be made!",
  "Error: Something went wrong!"                                       :   "Something went wrong!",
  "VALIDATION_FAILED"                                                  :   "Something went wrong!",
  "Bad credentials"                                                    :   "Incorrect user name or password",
  "Password matches one of %1$s previous passwords."                   :   "Try again with a password you haven`t used before",
  "Passwords do not match!"                                            :   "Passwords do not match",
  "Incorrect current password"                                         :   "Current password is incorrect",
  "Something went wrong"                                               :   "Something went wrong!",
  "Error: Email is already in use!"                                    :   "Email is already in use!",
  "Error: Invalidate Pin-Code! User Not Approved"                      :   "The Verification Code does not match",
  "Error: Unable to save your changes"                                 :   "Unable to save your changes",
  "User account is locked"                                             :   "Your account has been locked",
  "size must be between 2 and 20"                                      :   "Enter a value between 2 to 20 characters long",
  "Unknown User"                                                       :   "Unknown User",
  "Details Saved"                                                      :   "Your account has been locked",
  "Password matches one of 4 previous passwords."                      :   "Password matches one of 4 previous passwords.",
  "Password must contain 1 or more special characters."                :   "Password must contain 1 or more special characters.",
  "* your phone number has been updated"                               :   "* Your phone number has been updated",
  "* your email address has been updated"                              :   "* Your email address has been updated",
  "Error: Your changes could not be saved"                             :   'Error: Your changes could not be saved',

  "Reset Password"                                                     :   "Reset Password",
  "Send me a temporary password once again"                            :   "Send me a temporary password once again",
  "A new temporary password has been sent to"                          :   "A new temporary password has been sent to",
  "your email address"                                                 :   "your email address",
  "3 Days to expire"                                                   :   "3 Days to expire",
  "A minimum of 8 characters"                                          :   "A minimum of 8 characters",
  "At least 1 Uppercase letters"                                       :   "At least 1 Uppercase letters",
  "At least 1 Lowercase letters"                                       :   "At least 1 Lowercase letters",
  "A number"                                                           :   "A number",
  " No more than 2 adjacent identical characters"                      :   " No more than 2 adjacent identical characters",
  "APP CENTER"                                                         :   "APP CENTER",
  "Admin"                                                              :   "Admin",
  "Aeonix App Center"                                                  :   "Aeonix App Center",
  "Agent"                                                              :   "Agent",
  "CHANGE PASSWORD"                                                    :   "CHANGE PASSWORD",
  "Change Password"                                                    :   "Change Password",
  "Change Password and Login"                                          :   "Change Password and Login",
  "Change it now >"                                                    :   "Change it now >",
  "Change the Initial Password"                                        :   "Change the Initial Password",
  "Chinese"                                                            :   "Chinese",
  "Complete Registration"                                              :   "Complete Registration",
  "Continue"                                                           :   "Continue",
  "EMAIL"                                                              :   "Email",
  "Enable Profile Picture"                                             :   "Enable Profile Picture",
  "English"                                                            :   "English",
  "Ex. Saul Ramirez"                                                   :   "Ex. Saul Ramirez",
  "Ex: +972547762084"                                                  :   "Ex: +972547762084",
  "Ex: abc@example.com"                                                 :   "Ex: abc@example.com",
  "FORGOT PASSWORD"                                                    :   "FORGOT PASSWORD",
  "Forgot Password?"                                                   :   "Forgot Password?",
  "Script-Designer"                                                    :   "Script Designer",
  "Hebrew"                                                             :   "Hebrew",
  "INITIAL PASSWORD"                                                   :   "password",
  "LOGIN SCREEN"                                                       :   "LOGIN SCREEN",
  "Log out"                                                            :   "Log out",
  "Login"                                                              :   "Login",
  "My Account"                                                         :   "My Account",
  "NEW PASSWORD"                                                       :   "NEW Password",
  "OLD PASSWORD"                                                       :   "OLD PASSWORD",
  "PASSWORD"                                                           :   "Password",
  "Please enter the Verification Code"                                 :   "Please enter the Verification Code",
  "you received in your mail:"                                         :   "you received in your mail:",
  "Please enter your User Name:"                                       :   "Please enter your User Name:",
  "Send Verification Code again"                                       :   "Send Verification Code again",
  "REGISTRATION"                                                       :   "REGISTRATION",
  "Russian"                                                            :   "Russian",
  "Send me a Verification Code"                                        :   "Send me a Verification Code",
  "Send me a Temporary Password"                                       :   "Send me a Temporary Password",
  "Spanish"                                                            :   "Spanish",
  "The password must contain:"                                         :   "The password must contain:",
  "USER DETAILS"                                                       :   "USER DETAILS",
  "USER NAME"                                                          :   "USERNAME",
  "WebRT"                                                              :   "Real-Time Reports",
  "You have successfully changed your password!"                       :   "You have successfully changed your password!",
  "You have successfully completed your registration!"                 :   "You have successfully completed your registration!",
  "Your password has expired and must be changed"                      :   "Your password has expired and must be changed",
  "Your password will expire in "                                      :   "Your password will expire in ",
  " Days."                                                             :   " Days.",
  "Complete the registration by filling the missing info:"             :   "Complete the registration by filling the missing info:",
  "PHONE NUMBER FOR AUTHENTICATION"                                    :   "PHONE NUMBER FOR AUTHENTICATION",
  "RE-ENTER NEW PASSWORD"                                              :   "re-enter new Password",
  "resend the verification code"                                       :   "resend the verification code",
  "Strength : "                                                        :   "Strength : ",
  "Must contain phone:"                                                :   "Must contain phone:",
  "A well-formed phone number"                                         :   "A well-formed phone number",
  "Must contain email:"                                                :   "Must contain email:",
  "A well-formed email address"                                        :   "A well-formed email address",
  "Successfully sent"                                                  :   "Successfully sent",
  "Sending failed!"                                                    :   "Sending failed!",
  "Back"                                                               :   "Back",

};
export const LANG_HEB_NAME = 'heb';
export const LANG_HEB_TRANS = {
  "Example"                                                            :   "דוגמא",
  "must be a well-formed email address"                                :   "must be a well-formed email address",
  "Error: A registry process should be made!"                          :   "יש להשלים תהליך רישום",
  "Error: Something went wrong!"                                       :   "!משהו השתבש",
  "VALIDATION_FAILED"                                                  :   "תהליך רישום לא תקין",
  "Bad credentials"                                                    :   "שם משתמש או סיסמא שגויים",
  "Password matches one of %1$s previous passwords."                   :   "בחר סיסמא חדשה שלא הייתה בשימוש",
  "Passwords do not match!"                                            :   "'הקפד להזין סיסמאות זהות בשדות 'סיסמה חדשה' ו'אימות סיסמה",
  "Incorrect current password"                                         :   "סיסמה נוכחית שגויה",
  "Something went wrong"                                               :   "!משהו השתבש",
  "Error: Email is already in use!"                                    :   'כתובת דוא"ל נמצאת בשימוש אצל משתמש קיים',
  "Error: Invalidate Pin-Code! User Not Approved"                      :   "קוד אימות שגוי",
  "Error: Unable to save your changes"                                 :   "לא ניתן לשמור את השינויים",
  "User account is locked"                                             :   "חשבונך ננעל",
  "size must be between 2 and 20"                                      :   "יש להזין ערך המכיל בין 2 ל-20 תווים",
  "Unknown User"                                                       :   "המשתמש אינו מוכר למערכת",
  "Details Saved"                                                      :   "חשבונך ננעל",
  "Password matches one of 4 previous passwords."                      :   "הסיסמה תואמת לאחת מ-4 סיסמאות קודמות",
  "Password must contain 1 or more special characters."                :   "הסיסמה חייבת להכיל 1 או יותר תווים בהצלחה מיוחדים",
  "* your phone number has been updated"                               :   "* מספר הטלפון שלך עודכן בהצלחה",
  "* your email address has been updated"                              :   '* כתובת הדוא"ל שלך עודכן בהצלחה',
  "Error: Your changes could not be saved"                             :   'שגיאה: השינויים שהזנת אינם נשמרו',

  "Reset Password"                                                     :   "איפוס סיסמה",
  "Send me a temporary password once again"                            :   "שלח לי שוב סיסמה זמנית",
  "A new temporary password has been sent to"                          :   "סיסמה זמנית חדשה נשלחה אל",
  "your email address"                                                 :   "כתובת הדוא\"ל שלך",
  "% Days to expire"                                                   :   "בעוד % ימים יפוג תוקף הסיסמה",
  " A minimum of 8 characters"                                         :   "מינימום 8 ספרות",
  " At least 1 Uppercase letters"                                      :   "לפחות אות אחת גדולה",
  " At least 1 lowercase letters"                                      :   "לפחות אות אחת קטנה",
  " At least 1 special character"                                      :   "לפחות תו מיוחד אחד",
  " A number"                                                          :   "מספר",
  " No more than 2 adjacent identical characters"                      :   "לא יותר מ-2 תווים עוקבים זהים",
  "APP CENTER"                                                         :   "APP CENTER",
  "Admin"                                                              :   "אדמיניסטרציה",
  "Aeonix App Center"                                                  :   "Aeonix App Center",
  "Agent"                                                              :   "נציג",
  "CHANGE PASSWORD"                                                    :   "שנה סיסמה",
  "Change Password"                                                    :   "שנה סיסמה",
  "Change Password and Login"                                          :   "שנה סיסמה והיכנס",
  "Change it now >"                                                    :   " <שנה עכשיו",
  "Change the Initial Password"                                        :   "שנה את הסיסמה הראשונית",
  "Chinese"                                                            :   "סינית",
  "Complete Registration"                                              :   "השלם תהליך רישום",
  "Continue"                                                           :   "המשך",
  "EMAIL"                                                              :   'כתובת דוא"ל',
  "Enable Profile Picture"                                             :   "אפשר תמונת פרופיל",
  "English"                                                            :   "אנגלית",
  "Ex. Saul Ramirez"                                                   :   "Ex. Saul Ramirez",
  "Ex: +972547762084"                                                  :   "Ex: +972547762084",
  "Ex: abc@example.com"                                                :   "Ex: abc@example.com",
  "FORGOT PASSWORD"                                                    :   "שכחתי סיסמה",
  "Forgot Password?"                                                   :   "שכחתי סיסמה",
  "Script-Designer"                                                    :   "עורך תסריטים",
  "Hebrew"                                                             :   "עברית",
  "INITIAL PASSWORD"                                                   :   "סיסמה ראשונית",
  "LOGIN SCREEN"                                                       :   "מסך כניסה",
  "Log out"                                                            :   "יציאה",
  "Login"                                                              :   "כניסה",
  "My Account"                                                         :   "החשבון שלי",
  "NEW PASSWORD"                                                       :   "סיסמה חדשה",
  "OLD PASSWORD"                                                       :   "סיסמה ישנה",
  "PASSWORD"                                                           :   "סיסמה",
  "Please enter the Verification Code"                                 :   "אנא הקלד את קוד האימות",
  "you received in your mail:"                                         :   'שקיבלת בדוא"ל',
  "Please enter your User Name:"                                       :   "אנא הקלד את שם המשתמש שלך",
  "Send Verification Code again"                                       :   "שלח קוד אימות חדש",
  "REGISTRATION"                                                       :   "רישום",
  "Russian"                                                            :   "רוסית",
  "Send me a Verification Code"                                        :   "שלח לי קוד אימות",
  "Send me a Temporary Password"                                       :   "שלח לי סיסמה זמנית",
  "Spanish"                                                            :   "ספרדית",
  "The password must contain:"                                         :   "על הסיסמה לכלול:",
  "USER DETAILS"                                                       :   "פרטי משתמש",
  "USER NAME"                                                          :   "שם משתמש",
  "WebRT"                                                              :   "דוחות זמן אמת",
  "You have successfully changed your password!"                       :   "סיסמתך שונתה בהצלחה",
  "You have successfully completed your registration!"                 :   "תהליך הרישום הסתיים בהצלחה",
  "Your password has expired and must be changed"                      :   "פג תוקף סיסמתך. עליך לשנותה",
  "Your password will expire in "                                      :   "תוקף סיסמתך יפוג בתוך ",
  " Days."                                                             :   " ימים",
  "Complete the registration by filling the missing info:"             :   "הקלד את המידע הבא לצורך השלמת תהליך הרישום",
  "PHONE NUMBER FOR AUTHENTICATION"                                    :   "מספר טלפון לאימות",
  "RE-ENTER NEW PASSWORD"                                              :   "האימות סיסמה",
  "resend the verification code"                                       :   "שלח שוב קוד אימות",
  "Strength : "                                                        :   "חוזק : ",
  "Must contain phone:"                                                :   "חייב לכלול מספר טלפון",
  " A well-formed phone number"                                        :   "מספר טלפון תקני",
  "Must contain email:"                                                :   'חייב לכלול כתובת דוא"ל',
  " A well-formed email address"                                       :   'כתובת דוא"ל תקנית',
  "Settings"                                                           :   'הגדרות',
  "Successfully sent"                                                  :   "נשלח בהצלחה",
  "Sending failed!"                                                    :   "!שליחה נכשלה",
  "Back"                                                               :   "חזור",

};
export const LANG_ESP_NAME = 'esp';
export const LANG_ESP_TRANS = {
  "Example"                                                            :   "Ejemplo",
  "must be a well-formed email address"                                :   "debe ser una dirección de correo electrónica bien formada",
  "Error: A registry process should be made!"                          :   "¡Se debe completar un proceso de registro!",
  "Error: Something went wrong!"                                       :   "Algo salió mal!",
  "VALIDATION_FAILED"                                                  :   "¡El proceso de registro no válido!",
  "Bad credentials"                                                    :   "Usuario o contraseña incorrecta",
  "Password matches one of %1$s previous passwords."                   :   "Vuelve a intentarlo con una contraseña que no hayas usado antes",
  "Passwords do not match!"                                            :   "¡Las contraseñas no coinciden",
  "Incorrect current password"                                         :   "Contraseña actual incorrecta",
  "Something went wrong"                                               :   "Algo salió mal!",
  "Error: Email is already in use!"                                    :   "¡Correo electrónico ya está en uso!",
  "Error: Invalidate Pin-Code! User Not Approved"                      :   "El código de verificación no coincide",
  "Error: Unable to save your changes"                                 :   "No se pueden guardar los cambios",
  "User account is locked"                                             :   "Su cuenta ha sido bloqueada",
  "size must be between 2 and 20"                                      :   "Ingrese un valor entre 2 y 20 caracteres de largo",
  "Unknown User"                                                       :   "Usuario Desconocido",
  "Details Saved"                                                      :   "Su cuenta ha sido bloqueada",
  "Password matches one of 4 previous passwords."                      :   "La contraseña coincide con una de las 4 contraseñas anteriores.",
  "Password must contain 1 or more special characters."                :   "La contraseña debe contener 1 o más caracteres especiales.",
  "* your phone number has been updated"                               :   "* Su número de teléfono ha sido actualizado",
  "* your email address has been updated"                              :   "* Su dirección de correo electrónico ha sido actualizada",

  "Reset Password"                                                     :   "Restablecer la contraseña",
  "Send me a reset password once again"                                :   "Envíame un contraseña temporal una vez más",
  "A new temporary password has been sent to"                          :   "Se ha enviado una nueva contraseña temporal a",
  "your email address"                                                 :   "Su dirección de correo electrónico",
  "3 Days to expire"                                                   :   "3 días para caducar",
  " A minimum of 8 characters"                                         :   "Un mínimo de 8 caracteres",
  " At least 1 Uppercase letters"                                      :   "Al menos 1 letras mayúsculas",
  " At least 1 lowercase letters"                                      :   "Al menos 1 letras minúsculas",
  " At least 1 special character"                                      :   "Al menos 1 letras minúsculas",
  " A number"                                                          :   "Un número",
  " No more than 2 adjacent identical characters"                      :   " No más de 2 caracteres consecutivos idénticos",
  "APP CENTER"                                                         :   "CENTRO DE APLICACIONES",
  "Admin"                                                              :   "Administrador",
  "Aeonix App Center"                                                  :   "Centro de aplicaciones Aeonix",
  "Agent"                                                              :   "Agente",
  "CHANGE PASSWORD"                                                    :   "CAMBIAR LA CONTRASEÑA",
  "Change Password"                                                    :   "Cambiar la contraseña",
  "Change Password and Login"                                          :   "Cambiar contraseña e iniciar sesión",
  "Change it now >"                                                    :   "Cámbialo ahora >",
  "Change the Initial Password"                                        :   "Cambiar la contraseña inicial",
  "Chinese"                                                            :   "Chino",
  "Complete Registration"                                              :   "Registracion completa",
  "Continue"                                                           :   "Continuar",
  "EMAIL"                                                              :   "Correo electrónico",
  "Enable Profile Picture"                                             :   "Habilitar imagen de perfil",
  "English"                                                            :   "Inglés",
  "Ex. Saul Ramirez"                                                   :   "Ej. Saúl Ramírez",
  "Ex: +972547762084"                                                  :   "Ej: +972547762084",
  "Ex: abc@example.com"                                                :   "Ej: abc@ejemplo.com",
  "FORGOT PASSWORD"                                                    :   "HAS OLVIDADO TU CONTRASEÑA",
  "Forgot Password?"                                                   :   "¿Has olvidado tu contraseña?",
  "Script-Designer"                                                    :   "Diseñador de guiones",
  "Hebrew"                                                             :   "Hebreo",
  "INITIAL PASSWORD"                                                   :   "Contraseña inicial",
  "LOGIN SCREEN"                                                       :   "PANTALLA DE INGRESO AL SISTEMA",
  "Log out"                                                            :   "Cerrar sesión",
  "Login"                                                              :   "Iniciar sesión",
  "My Account"                                                         :   "Mi cuenta",
  "NEW PASSWORD"                                                       :   "Nueva contraseña",
  "OLD PASSWORD"                                                       :   "CONTRASEÑA ANTERIOR",
  "PASSWORD"                                                           :   "Contraseña",
  "Please enter the Verification Code"                                 :   "Porfavor ingrese el codigo de verificacion",
  "you received in your mail:"                                         :   "Lo recibiste en tu correo:",
  "Please enter your User Name:"                                       :   "Por favor, ingrese su usuario:",
  "Send Verification Code again"                                       :   "Enviar código de verificación de nuevo",
  "REGISTRATION"                                                       :   "REGISTRACION",
  "Russian"                                                            :   "Ruso",
  "Send me a Verification Code"                                        :   "Envíame un código de verificación",
  "Send me a Temporary Password"                                       :   "Envíame un contraseña temporal",
  "Spanish"                                                            :   "Español",
  "The password must contain:"                                         :   "La contraseña debe contener:",
  "USER DETAILS"                                                       :   "DETALLES DE USUARIO",
  "USER NAME"                                                          :   "USUARIO",
  "WebRT"                                                              :   "WebRT",
  "You have successfully changed your password!"                       :   "¡Has cambiado satisfactoriamente tu contraseña!",
  "You have successfully completed your registration!"                 :   "¡Has completado con éxito tu registro!",
  "Your password has expired and must be changed"                      :   "Su contraseña ha expirado y debe cambiarse",
  "Your password will expire in "                                      :   "Su contraseña expíra en ",
  " Days."                                                             :   " días.",
  "Complete the registration by filling the missing info:"             :   "Complete el registro llenando la siguiente información:",
  "PHONE NUMBER FOR AUTHENTICATION"                                    :   "NÚMERO DE TELÉFONO PARA AUTENTICACIÓN",
  "RE-ENTER NEW PASSWORD"                                              :   "re-ingrese nueva contraseña",
  "resend the verification code"                                       :   "reenviar el código de verificación",
  "Strength : "                                                        :   "fortaleza : ",
  "Must contain phone:"                                                :   "Debe contener número de teléfono:",
  "A well-formed phone number"                                         :   "Un número de teléfono bien formado",
  "Must contain email:"                                                :   "Debe contener correo electrónico:",
  "A well-formed email address"                                        :   "Una dirección de correo electrónico bien formada",
  "Successfully sent"                                                  :   "Enviado exitosamente",
  "Sending failed!"                                                    :   "¡Envío fallido!",
  "Back"                                                               :   "Atrás",
};
export const LANG_CHI_NAME = 'chi';
export const LANG_CHI_TRANS = {
  "Example"                                                            :   "例如",
  "must be a well-formed email address"                                :   "请输入正确的邮箱地址",
  "Error: A registry process should be made!"                          :   "应该先完成注册！",
  "Error: Something went wrong!"                                       :   "出了些问题！",
  "VALIDATION_FAILED"                                                  :   "注册无效！",
  "Bad credentials"                                                    :   "用户名或密码有误",
  "Password matches one of %1$s previous passwords."                   :   "请用一个以前没用过的密码再试一次",
  "Passwords do not match!"                                            :   "密码不符",
  "Incorrect current password"                                         :   "当前密码不正确",
  "Something went wrong"                                               :   "出了些问题！",
  "Error: Email is already in use!"                                    :   "此邮箱已被使用！",
  "Error: Invalidate Pin-Code! User Not Approved"                      :   "验证码不符",
  "Error: Unable to save your changes"                                 :   "无法保存修改的内容",
  "User account is locked"                                             :   "您的账号已被锁定",
  "size must be between 2 and 20"                                      :   "输入 2 到 20 个字符长的值",
  "Unknown User"                                                       :   "输入 2 到 20 个字符长的值",
  "Details Saved"                                                      :   "您的账号已被锁定",
  "Password matches one of 4 previous passwords."                      :   "密码与之前 4 个密码中的一个相匹配。",
  "Password must contain 1 or more special characters."                :   "密码必须包含 1 个或多个特殊字符。",
  "* your phone number has been updated"                               :   "*您的电话号码已更新",
  "* your email address has been updated"                              :   "*您的电子邮件地址已更新",

  "Reset Password"                                                     :   "重设密码",
  "Send me a temporary password once again"                            :   "再次向我发送临时密码",
  "A new temporary password has been sent to"                          :   "新的临时密码已发送至",
  "your email address"                                                 :   "您的电子邮件地址",
  "3 Days to expire"                                                   :   "三天后到期",
  " A minimum of 8 characters"                                         :   "至少包含8个字符",
  " At least 1 Uppercase letters"                                      :   "至少包含1个大写字母",
  " At least 1 lowercase letters"                                      :   "至少包含1个小写字母",
  " At least 1 special character"                                      :   "至少包含1个特殊字符",
  " A number"                                                          :   "一个数字",
  " No more than 2 adjacent identical characters"                      :   " 不超过 2 个相同的连续字符",
  "APP CENTER"                                                         :   "应用中心",
  "Admin"                                                              :   "管理员",
  "Aeonix App Center"                                                  :   "Aeonix 应用中心",
  "Agent"                                                              :   "坐席",
  "CHANGE PASSWORD"                                                    :   "修改密码",
  "Change Password"                                                    :   "修改密码",
  "Change Password and Login"                                          :   "修改密码然后登录",
  "Change it now >"                                                    :   "立即修改>",
  "Change the Initial Password"                                        :   "修改初始密码",
  "Chinese"                                                            :   "中文",
  "Complete Registration"                                              :   "完成注册",
  "Continue"                                                           :   "继续",
  "EMAIL"                                                              :   "电子邮件",
  "Enable Profile Picture"                                             :   "使用个人资料图片",
  "English"                                                            :   "英语",
  "Ex. Saul Ramirez"                                                   :   "例如：Saul Ramirez",
  "Ex: +972547762084"                                                  :   "例如：+972547762084",
  "Ex: abc@example.com"                                                :   "例如：abc@example.com",
  "FORGOT PASSWORD"                                                    :   "忘记密码",
  "Forgot Password?"                                                   :   "忘记密码？",
  "Script-Designer"                                                    :   "脚本设计器",
  "Hebrew"                                                             :   "希伯来语",
  "INITIAL PASSWORD"                                                   :   "初始密码",
  "LOGIN SCREEN"                                                       :   "登录界面",
  "Log out"                                                            :   "注销",
  "Login"                                                              :   "登录",
  "My Account"                                                         :   "我的账户",
  "NEW PASSWORD"                                                       :   "新密码",
  "OLD PASSWORD"                                                       :   "旧密码",
  "PASSWORD"                                                           :   "密码",
  "Please enter the Verification Code"                                 :   "请输入验证码",
  "you received in your mail:"                                         :   "你在邮件中收到的：",
  "Please enter your User Name:"                                       :   "请输入你的用户名：",
  "Send Verification Code again"                                       :   "再次发送验证码",
  "REGISTRATION"                                                       :   "注册",
  "Russian"                                                            :   "俄语",
  "Send me a Verification Code"                                        :   "给我发送验证码",
  "Send me a Temporary Password"                                       :   "给我临时密码",
  "Spanish"                                                            :   "西班牙语",
  "The password must contain:"                                         :   "密码必须包含：",
  "USER DETAILS"                                                       :   "用户信息",
  "USER NAME"                                                          :   "用户名",
  "WebRT"                                                              :   "实时报告",
  "You have successfully changed your password!"                       :   "您已成功修改密码！",
  "You have successfully completed your registration!"                 :   "您已成功完成注册！",
  "Your password has expired and must be changed"                      :   "您的密码已过期，必须修改",
  "Your password will expire in "                                      :   "您的密码将在",
  " Days."                                                             :   "天后过期。",
  "Complete the registration by filling the missing info:"             :   "通过填写以下信息完成注册：",
  "PHONE NUMBER FOR AUTHENTICATION"                                    :   "输入电话号码进行验证",
  "RE-ENTER NEW PASSWORD"                                              :   "重新输入新密码",
  "resend the verification code"                                       :   "重新发送验证码",
  "Strength : "                                                        :   "强度 : ",
  "Must contain phone:"                                                :   "必须包含电话：",
  "A well-formed phone number"                                         :   "一个正确的电话号码",
  "Must contain email:"                                                :   "必须包含电邮地址：",
  "A well-formed email address"                                        :   "一个正确的电子邮件地址",
  "Successfully sent"                                                  :   "发送成功",
  "Sending failed!"                                                    :   "发送失败！",
  "Back"                                                               :   "后退",
};
export const LANG_RUS_NAME = 'rus';
export const LANG_RUS_TRANS = {
  "Example"                                                            :   "Пример",
  "must be a well-formed email address"                                :   "должен быть корректно оформленным адресом электронной почты",
  "Error: A registry process should be made!"                          :   "Должен быть проведен процесс регистрации!",
  "Error: Something went wrong!"                                       :   "Что-то пошло не так!",
  "VALIDATION_FAILED"                                                  :   "Процесс реестра недействителен!",
  "Bad credentials"                                                    :   "Неверное имя пользователя или пароль",
  "Password matches one of %1$s previous passwords."                   :   "Попробуйте еще раз с паролем, который вы раньше не использовали",
  "Passwords do not match!"                                            :   "Пароли не совпадают",
  "Incorrect current password"                                         :   "Неверный текущий пароль",
  "Something went wrong"                                               :   "Что-то пошло не так!",
  "Error: Email is already in use!"                                    :   "Email уже используется!",
  "Error: Invalidate Pin-Code! User Not Approved"                      :   "Код подтверждения не совпадает",
  "Error: Unable to save your changes"                                 :   "Не удалось сохранить изменения",
  "User account is locked"                                             :   "Ваш аккаунт заблокирован",
  "size must be between 2 and 20"                                      :   "Введите значение длиной от 2 до 20 символов.",
  "Unknown User"                                                       :   "Неизвестный пользователь",
  "Details Saved"                                                      :   "Ваш аккаунт заблокирован",
  "Password matches one of 4 previous passwords."                      :   "Пароль совпадает с одним из 4 предыдущих паролей.",
  "Password must contain 1 or more special characters."                :   "Пароль должен содержать 1 или более специальных символов.",
  "* your phone number has been updated"                               :   "* Ваш номер телефона был обновлен",
  "* your email address has been updated"                              :   "* Ваш адрес электронной почты был обновлен",


  "Reset Password"                                                     :   "Сброс пароля",
  "Send me a temporary password once again"                            :   "Отправьте мне временный пароль еще раз",
  "A new temporary password has been sent to"                          :   "Новый временный пароль был отправлен на",
  "your email address"                                                 :   "ваш адрес электронной почты",
  "3 Days to expire"                                                   :   "3 días para caducar",
  "A minimum of 8 characters"                                          :   "Минимум 8 символов",
  "At least 1 Uppercase letters"                                       :   "Минимум 1 заглавная буква",
  "At least 1 Lowercase letters"                                       :   "Минимум 1 строчная буква",
  "A number"                                                           :   "Число",
  " No more than 2 adjacent identical characters"                      :   " Не более 2 одинаковых последовательных символов",
  "APP CENTER"                                                         :   "ЦЕНТР ПРИЛОЖЕНИЙ",
  "Admin"                                                              :   "Админ",
  "Aeonix App Center"                                                  :   "Центр приложений Aeonix",
  "Agent"                                                              :   "Агент",
  "CHANGE PASSWORD"                                                    :   "ИЗМЕНИТЬ ПАРОЛЬ",
  "Change Password"                                                    :   "Изменить пароль",
  "Change Password and Login"                                          :   "Изменить пароль и логин",
  "Change it now >"                                                    :   "Измените его сейчас >",
  "Change the Initial Password"                                        :   "Изменить начальный пароль",
  "Chinese"                                                            :   "Китайский язык",
  "Complete Registration"                                              :   "Завершите регистрацию",
  "Continue"                                                           :   "Продолжать",
  "EMAIL"                                                              :   "Электронная почта",
  "Enable Profile Picture"                                             :   "Включить изображение профиля",
  "English"                                                            :   "Английский",
  "Ex. Saul Ramirez"                                                   :   "Бывший. Саул Рамирес",
  "Ex: +972547762084"                                                  :   "Пример: +972547762084",
  "Ex: abc@example.com"                                                :   "Пример: abc@example.com",
  "FORGOT PASSWORD"                                                    :   "ЗАБЫЛИ ПАРОЛЬ",
  "Forgot Password?"                                                   :   "Забыли пароль?",
  "Script-Designer"                                                    :   "Сценарист",
  "Hebrew"                                                             :   "Иврит",
  "INITIAL PASSWORD"                                                   :   "пароль",
  "LOGIN SCREEN"                                                       :   "ЭКРАН ВХОДА",
  "Log out"                                                            :   "Выйти",
  "Login"                                                              :   "Авторизоваться",
  "My Account"                                                         :   "Мой счет",
  "NEW PASSWORD"                                                       :   "Новый пароль",
  "OLD PASSWORD"                                                       :   "СТАРЫЙ ПАРОЛЬ",
  "PASSWORD"                                                           :   "Пароль",
  "Please enter the Verification Code"                                 :   "Пожалуйста, введите проверочный код",
  "you received in your mail:"                                         :   "вы получили на почту:",
  "Please enter your User Name:"                                       :   "Пожалуйста, введите Ваш логин:",
  "Send Verification Code again"                                       :   "Отправить код подтверждения еще раз",
  "REGISTRATION"                                                       :   "ПОСТАНОВКА НА УЧЕТ",
  "Russian"                                                            :   "Русский",
  "Send me a Verification Code"                                        :   "Пришлите мне код подтверждения",
  "Send me a Temporary Password"                                       :   "Пришлите мне временный пароль",
  "Spanish"                                                            :   "Испанский язык",
  "The password must contain:"                                         :   "Пароль должен содержать:",
  "USER DETAILS"                                                       :   "ДАННЫЕ ПОЛЬЗОВАТЕЛЯ",
  "USER NAME"                                                          :   "ИМЯ ПОЛЬЗОВАТЕЛЯ",
  "WebRT"                                                              :   "ВебРТ",
  "You have successfully changed your password!"                       :   "Вы успешно изменили ваш пароль!",
  "You have successfully completed your registration!"                 :   "Вы успешно завершили регистрацию!",
  "Your password has expired and must be changed"                      :   "Срок действия вашего пароля истек, и его необходимо изменить",
  "Your password will expire in "                                      :   "Срок действия вашего пароля истечет через ",
  " Days."                                                             :   "  Дни.",
  "Complete the registration by filling the missing info:"             :   "Завершите регистрацию, заполнив недостающую информацию:",
  "PHONE NUMBER FOR AUTHENTICATION"                                    :   "НОМЕР ТЕЛЕФОНА ДЛЯ АУТЕНТИФИКАЦИИ",
  "RE-ENTER NEW PASSWORD"                                              :   "Повторно введите новый пароль",
  "resend the verification code"                                       :   "повторно отправить код подтверждения",
  "Strength : "                                                        :   "Сила : ",
  "Must contain phone:"                                                :   "Должен содержать телефон:",
  "A well-formed phone number"                                         :   "Правильно сформированный номер телефона",
  "Must contain email:"                                                :   "Должен содержать электронную почту:",
  "A well-formed email address"                                        :   "Правильно сформированный адрес электронной почты",
  "Successfully sent"                                                  :   "Успешно отправлено",
  "Sending failed!"                                                    :   "Отправка не удалась!",
  "Back"                                                               :   "Назад",

};



export declare type Direction = 'ltr' | 'rtl';
export const dictionary = {
  [LANG_ENG_NAME]: {dir: 'ltr', trans: LANG_ENG_TRANS},
  [LANG_HEB_NAME]: {dir: 'rtl', trans: LANG_HEB_TRANS},
  [LANG_ESP_NAME]: {dir: 'ltr', trans: LANG_ESP_TRANS},
  [LANG_CHI_NAME]: {dir: 'ltr', trans: LANG_CHI_TRANS},
  [LANG_RUS_NAME]: {dir: 'ltr', trans: LANG_RUS_TRANS},

};
export const TRANSLATION_PROVIDERS = [
  { provide: TRANSLATIONS, useValue: dictionary },
];




@Injectable({
  providedIn: 'root'
})
@Directive({
  selector: 'translate',
})
export class TranslateService implements OnInit {
  private _currentLang: string = AppConfig.translateLanguage.lang;
  private _currentDir: Direction = 'ltr';
  public get currentLang() {
    return this._currentLang;
  }

  public get currentDir(): Direction {
    //console.log("this.currentLang:" + this._currentLang)
    return this._currentDir;
  }

  constructor(@Inject(TRANSLATIONS) private _translations: any,
              /*private translations: translateRememberLangComponent,
              private tokenService: TokenStorageService*/) {
    //this.getLang();
  }

  ngOnInit() {
    //this.getLang();
  }

  getLang() {
    //const {_currentLang, _currentDir} = this.translations.getLang(this._translations).values
    this._currentLang = AppConfig.translateLanguage.lang; //_currentLang;
    this._currentDir = (AppConfig.translateLanguage.dir.match('ltr') ? 'ltr' : 'rtl'); //_currentDir;
  }

  setLang(lang: string) {
    //this.translations.setLang(lang);
    AppConfig.translateLanguage.lang = this._currentLang.toString();
    AppConfig.translateLanguage.dir = this._currentDir.toString();
  }

/*  getLang() {
    let lang = 'eng';
    let username = null;
    if (this.tokenService?.getUser() != null ) {username = this.tokenService.getUsername();}
    this.userService.getLanguage((username != null ? username : '')).subscribe(
      data => {
        console.log('get language: '+data.data)
        AppConfig.translateLanguage.lang = data.data;
        this._currentLang = (data.data);
        this._currentDir = (this._translations[data.data]?.dir ?? 'ltr');
        lang = data.data
      },
      err => {
        console.log(JSON.parse(err.error).message);
      }
    );
    return lang;
  }

  setLang(lang: string) {
    if (lang) {
      const username = this.tokenService?.getUsername();
      this.userService.setLanguage(lang, (username != null ? username : '')).subscribe(
        data => {
          console.log(data.message);
        },
        err => {
          console.log(JSON.parse(err.error).message);
        }
      );
    }
  }
*/
  public use(lang: string): void {
    this._currentLang = lang;
    //this.setLang(lang);
    this._currentDir = (this._translations[lang]?.dir ?? 'ltr'); //
  }

  public translate(key: string): string{
    if  (typeof(key) ==  undefined) {return "";}
    let translation = key;
    if (this._translations[this.currentLang] && this._translations[this.currentLang].trans[key]) {
      return this._translations[this.currentLang].trans[key];
    }
    return translation;
  }

  public instant(key: string) {
    return this.translate(key);
  }
}
