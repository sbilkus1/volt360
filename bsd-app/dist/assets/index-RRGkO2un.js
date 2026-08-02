(function(){const e=document.createElement("link").relList;if(e&&e.supports&&e.supports("modulepreload"))return;for(const o of document.querySelectorAll('link[rel="modulepreload"]'))i(o);new MutationObserver(o=>{for(const a of o)if(a.type==="childList")for(const u of a.addedNodes)u.tagName==="LINK"&&u.rel==="modulepreload"&&i(u)}).observe(document,{childList:!0,subtree:!0});function t(o){const a={};return o.integrity&&(a.integrity=o.integrity),o.referrerPolicy&&(a.referrerPolicy=o.referrerPolicy),o.crossOrigin==="use-credentials"?a.credentials="include":o.crossOrigin==="anonymous"?a.credentials="omit":a.credentials="same-origin",a}function i(o){if(o.ep)return;o.ep=!0;const a=t(o);fetch(o.href,a)}})();function cd(s){return s&&s.__esModule&&Object.prototype.hasOwnProperty.call(s,"default")?s.default:s}var gf={exports:{}},ra={},_f={exports:{}},mt={};/**
 * @license React
 * react.production.min.js
 *
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */var Zm;function Iy(){if(Zm)return mt;Zm=1;var s=Symbol.for("react.element"),e=Symbol.for("react.portal"),t=Symbol.for("react.fragment"),i=Symbol.for("react.strict_mode"),o=Symbol.for("react.profiler"),a=Symbol.for("react.provider"),u=Symbol.for("react.context"),c=Symbol.for("react.forward_ref"),h=Symbol.for("react.suspense"),d=Symbol.for("react.memo"),m=Symbol.for("react.lazy"),_=Symbol.iterator;function g(O){return O===null||typeof O!="object"?null:(O=_&&O[_]||O["@@iterator"],typeof O=="function"?O:null)}var S={isMounted:function(){return!1},enqueueForceUpdate:function(){},enqueueReplaceState:function(){},enqueueSetState:function(){}},M=Object.assign,E={};function y(O,ie,Ae){this.props=O,this.context=ie,this.refs=E,this.updater=Ae||S}y.prototype.isReactComponent={},y.prototype.setState=function(O,ie){if(typeof O!="object"&&typeof O!="function"&&O!=null)throw Error("setState(...): takes an object of state variables to update or a function which returns an object of state variables.");this.updater.enqueueSetState(this,O,ie,"setState")},y.prototype.forceUpdate=function(O){this.updater.enqueueForceUpdate(this,O,"forceUpdate")};function x(){}x.prototype=y.prototype;function b(O,ie,Ae){this.props=O,this.context=ie,this.refs=E,this.updater=Ae||S}var T=b.prototype=new x;T.constructor=b,M(T,y.prototype),T.isPureReactComponent=!0;var P=Array.isArray,H=Object.prototype.hasOwnProperty,D={current:null},N={key:!0,ref:!0,__self:!0,__source:!0};function z(O,ie,Ae){var Z,ne={},xe=null,de=null;if(ie!=null)for(Z in ie.ref!==void 0&&(de=ie.ref),ie.key!==void 0&&(xe=""+ie.key),ie)H.call(ie,Z)&&!N.hasOwnProperty(Z)&&(ne[Z]=ie[Z]);var ee=arguments.length-2;if(ee===1)ne.children=Ae;else if(1<ee){for(var we=Array(ee),Ue=0;Ue<ee;Ue++)we[Ue]=arguments[Ue+2];ne.children=we}if(O&&O.defaultProps)for(Z in ee=O.defaultProps,ee)ne[Z]===void 0&&(ne[Z]=ee[Z]);return{$$typeof:s,type:O,key:xe,ref:de,props:ne,_owner:D.current}}function C(O,ie){return{$$typeof:s,type:O.type,key:ie,ref:O.ref,props:O.props,_owner:O._owner}}function w(O){return typeof O=="object"&&O!==null&&O.$$typeof===s}function U(O){var ie={"=":"=0",":":"=2"};return"$"+O.replace(/[=:]/g,function(Ae){return ie[Ae]})}var X=/\/+/g;function k(O,ie){return typeof O=="object"&&O!==null&&O.key!=null?U(""+O.key):ie.toString(36)}function Y(O,ie,Ae,Z,ne){var xe=typeof O;(xe==="undefined"||xe==="boolean")&&(O=null);var de=!1;if(O===null)de=!0;else switch(xe){case"string":case"number":de=!0;break;case"object":switch(O.$$typeof){case s:case e:de=!0}}if(de)return de=O,ne=ne(de),O=Z===""?"."+k(de,0):Z,P(ne)?(Ae="",O!=null&&(Ae=O.replace(X,"$&/")+"/"),Y(ne,ie,Ae,"",function(Ue){return Ue})):ne!=null&&(w(ne)&&(ne=C(ne,Ae+(!ne.key||de&&de.key===ne.key?"":(""+ne.key).replace(X,"$&/")+"/")+O)),ie.push(ne)),1;if(de=0,Z=Z===""?".":Z+":",P(O))for(var ee=0;ee<O.length;ee++){xe=O[ee];var we=Z+k(xe,ee);de+=Y(xe,ie,Ae,we,ne)}else if(we=g(O),typeof we=="function")for(O=we.call(O),ee=0;!(xe=O.next()).done;)xe=xe.value,we=Z+k(xe,ee++),de+=Y(xe,ie,Ae,we,ne);else if(xe==="object")throw ie=String(O),Error("Objects are not valid as a React child (found: "+(ie==="[object Object]"?"object with keys {"+Object.keys(O).join(", ")+"}":ie)+"). If you meant to render a collection of children, use an array instead.");return de}function ae(O,ie,Ae){if(O==null)return O;var Z=[],ne=0;return Y(O,Z,"","",function(xe){return ie.call(Ae,xe,ne++)}),Z}function te(O){if(O._status===-1){var ie=O._result;ie=ie(),ie.then(function(Ae){(O._status===0||O._status===-1)&&(O._status=1,O._result=Ae)},function(Ae){(O._status===0||O._status===-1)&&(O._status=2,O._result=Ae)}),O._status===-1&&(O._status=0,O._result=ie)}if(O._status===1)return O._result.default;throw O._result}var ce={current:null},G={transition:null},he={ReactCurrentDispatcher:ce,ReactCurrentBatchConfig:G,ReactCurrentOwner:D};function re(){throw Error("act(...) is not supported in production builds of React.")}return mt.Children={map:ae,forEach:function(O,ie,Ae){ae(O,function(){ie.apply(this,arguments)},Ae)},count:function(O){var ie=0;return ae(O,function(){ie++}),ie},toArray:function(O){return ae(O,function(ie){return ie})||[]},only:function(O){if(!w(O))throw Error("React.Children.only expected to receive a single React element child.");return O}},mt.Component=y,mt.Fragment=t,mt.Profiler=o,mt.PureComponent=b,mt.StrictMode=i,mt.Suspense=h,mt.__SECRET_INTERNALS_DO_NOT_USE_OR_YOU_WILL_BE_FIRED=he,mt.act=re,mt.cloneElement=function(O,ie,Ae){if(O==null)throw Error("React.cloneElement(...): The argument must be a React element, but you passed "+O+".");var Z=M({},O.props),ne=O.key,xe=O.ref,de=O._owner;if(ie!=null){if(ie.ref!==void 0&&(xe=ie.ref,de=D.current),ie.key!==void 0&&(ne=""+ie.key),O.type&&O.type.defaultProps)var ee=O.type.defaultProps;for(we in ie)H.call(ie,we)&&!N.hasOwnProperty(we)&&(Z[we]=ie[we]===void 0&&ee!==void 0?ee[we]:ie[we])}var we=arguments.length-2;if(we===1)Z.children=Ae;else if(1<we){ee=Array(we);for(var Ue=0;Ue<we;Ue++)ee[Ue]=arguments[Ue+2];Z.children=ee}return{$$typeof:s,type:O.type,key:ne,ref:xe,props:Z,_owner:de}},mt.createContext=function(O){return O={$$typeof:u,_currentValue:O,_currentValue2:O,_threadCount:0,Provider:null,Consumer:null,_defaultValue:null,_globalName:null},O.Provider={$$typeof:a,_context:O},O.Consumer=O},mt.createElement=z,mt.createFactory=function(O){var ie=z.bind(null,O);return ie.type=O,ie},mt.createRef=function(){return{current:null}},mt.forwardRef=function(O){return{$$typeof:c,render:O}},mt.isValidElement=w,mt.lazy=function(O){return{$$typeof:m,_payload:{_status:-1,_result:O},_init:te}},mt.memo=function(O,ie){return{$$typeof:d,type:O,compare:ie===void 0?null:ie}},mt.startTransition=function(O){var ie=G.transition;G.transition={};try{O()}finally{G.transition=ie}},mt.unstable_act=re,mt.useCallback=function(O,ie){return ce.current.useCallback(O,ie)},mt.useContext=function(O){return ce.current.useContext(O)},mt.useDebugValue=function(){},mt.useDeferredValue=function(O){return ce.current.useDeferredValue(O)},mt.useEffect=function(O,ie){return ce.current.useEffect(O,ie)},mt.useId=function(){return ce.current.useId()},mt.useImperativeHandle=function(O,ie,Ae){return ce.current.useImperativeHandle(O,ie,Ae)},mt.useInsertionEffect=function(O,ie){return ce.current.useInsertionEffect(O,ie)},mt.useLayoutEffect=function(O,ie){return ce.current.useLayoutEffect(O,ie)},mt.useMemo=function(O,ie){return ce.current.useMemo(O,ie)},mt.useReducer=function(O,ie,Ae){return ce.current.useReducer(O,ie,Ae)},mt.useRef=function(O){return ce.current.useRef(O)},mt.useState=function(O){return ce.current.useState(O)},mt.useSyncExternalStore=function(O,ie,Ae){return ce.current.useSyncExternalStore(O,ie,Ae)},mt.useTransition=function(){return ce.current.useTransition()},mt.version="18.3.1",mt}var Jm;function fd(){return Jm||(Jm=1,_f.exports=Iy()),_f.exports}/**
 * @license React
 * react-jsx-runtime.production.min.js
 *
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */var Qm;function Uy(){if(Qm)return ra;Qm=1;var s=fd(),e=Symbol.for("react.element"),t=Symbol.for("react.fragment"),i=Object.prototype.hasOwnProperty,o=s.__SECRET_INTERNALS_DO_NOT_USE_OR_YOU_WILL_BE_FIRED.ReactCurrentOwner,a={key:!0,ref:!0,__self:!0,__source:!0};function u(c,h,d){var m,_={},g=null,S=null;d!==void 0&&(g=""+d),h.key!==void 0&&(g=""+h.key),h.ref!==void 0&&(S=h.ref);for(m in h)i.call(h,m)&&!a.hasOwnProperty(m)&&(_[m]=h[m]);if(c&&c.defaultProps)for(m in h=c.defaultProps,h)_[m]===void 0&&(_[m]=h[m]);return{$$typeof:e,type:c,key:g,ref:S,props:_,_owner:o.current}}return ra.Fragment=t,ra.jsx=u,ra.jsxs=u,ra}var eg;function Fy(){return eg||(eg=1,gf.exports=Uy()),gf.exports}var fe=Fy(),gt=fd();const Oy=cd(gt);var Il={},vf={exports:{}},Un={},yf={exports:{}},xf={};/**
 * @license React
 * scheduler.production.min.js
 *
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */var tg;function By(){return tg||(tg=1,(function(s){function e(G,he){var re=G.length;G.push(he);e:for(;0<re;){var O=re-1>>>1,ie=G[O];if(0<o(ie,he))G[O]=he,G[re]=ie,re=O;else break e}}function t(G){return G.length===0?null:G[0]}function i(G){if(G.length===0)return null;var he=G[0],re=G.pop();if(re!==he){G[0]=re;e:for(var O=0,ie=G.length,Ae=ie>>>1;O<Ae;){var Z=2*(O+1)-1,ne=G[Z],xe=Z+1,de=G[xe];if(0>o(ne,re))xe<ie&&0>o(de,ne)?(G[O]=de,G[xe]=re,O=xe):(G[O]=ne,G[Z]=re,O=Z);else if(xe<ie&&0>o(de,re))G[O]=de,G[xe]=re,O=xe;else break e}}return he}function o(G,he){var re=G.sortIndex-he.sortIndex;return re!==0?re:G.id-he.id}if(typeof performance=="object"&&typeof performance.now=="function"){var a=performance;s.unstable_now=function(){return a.now()}}else{var u=Date,c=u.now();s.unstable_now=function(){return u.now()-c}}var h=[],d=[],m=1,_=null,g=3,S=!1,M=!1,E=!1,y=typeof setTimeout=="function"?setTimeout:null,x=typeof clearTimeout=="function"?clearTimeout:null,b=typeof setImmediate<"u"?setImmediate:null;typeof navigator<"u"&&navigator.scheduling!==void 0&&navigator.scheduling.isInputPending!==void 0&&navigator.scheduling.isInputPending.bind(navigator.scheduling);function T(G){for(var he=t(d);he!==null;){if(he.callback===null)i(d);else if(he.startTime<=G)i(d),he.sortIndex=he.expirationTime,e(h,he);else break;he=t(d)}}function P(G){if(E=!1,T(G),!M)if(t(h)!==null)M=!0,te(H);else{var he=t(d);he!==null&&ce(P,he.startTime-G)}}function H(G,he){M=!1,E&&(E=!1,x(z),z=-1),S=!0;var re=g;try{for(T(he),_=t(h);_!==null&&(!(_.expirationTime>he)||G&&!U());){var O=_.callback;if(typeof O=="function"){_.callback=null,g=_.priorityLevel;var ie=O(_.expirationTime<=he);he=s.unstable_now(),typeof ie=="function"?_.callback=ie:_===t(h)&&i(h),T(he)}else i(h);_=t(h)}if(_!==null)var Ae=!0;else{var Z=t(d);Z!==null&&ce(P,Z.startTime-he),Ae=!1}return Ae}finally{_=null,g=re,S=!1}}var D=!1,N=null,z=-1,C=5,w=-1;function U(){return!(s.unstable_now()-w<C)}function X(){if(N!==null){var G=s.unstable_now();w=G;var he=!0;try{he=N(!0,G)}finally{he?k():(D=!1,N=null)}}else D=!1}var k;if(typeof b=="function")k=function(){b(X)};else if(typeof MessageChannel<"u"){var Y=new MessageChannel,ae=Y.port2;Y.port1.onmessage=X,k=function(){ae.postMessage(null)}}else k=function(){y(X,0)};function te(G){N=G,D||(D=!0,k())}function ce(G,he){z=y(function(){G(s.unstable_now())},he)}s.unstable_IdlePriority=5,s.unstable_ImmediatePriority=1,s.unstable_LowPriority=4,s.unstable_NormalPriority=3,s.unstable_Profiling=null,s.unstable_UserBlockingPriority=2,s.unstable_cancelCallback=function(G){G.callback=null},s.unstable_continueExecution=function(){M||S||(M=!0,te(H))},s.unstable_forceFrameRate=function(G){0>G||125<G?console.error("forceFrameRate takes a positive int between 0 and 125, forcing frame rates higher than 125 fps is not supported"):C=0<G?Math.floor(1e3/G):5},s.unstable_getCurrentPriorityLevel=function(){return g},s.unstable_getFirstCallbackNode=function(){return t(h)},s.unstable_next=function(G){switch(g){case 1:case 2:case 3:var he=3;break;default:he=g}var re=g;g=he;try{return G()}finally{g=re}},s.unstable_pauseExecution=function(){},s.unstable_requestPaint=function(){},s.unstable_runWithPriority=function(G,he){switch(G){case 1:case 2:case 3:case 4:case 5:break;default:G=3}var re=g;g=G;try{return he()}finally{g=re}},s.unstable_scheduleCallback=function(G,he,re){var O=s.unstable_now();switch(typeof re=="object"&&re!==null?(re=re.delay,re=typeof re=="number"&&0<re?O+re:O):re=O,G){case 1:var ie=-1;break;case 2:ie=250;break;case 5:ie=1073741823;break;case 4:ie=1e4;break;default:ie=5e3}return ie=re+ie,G={id:m++,callback:he,priorityLevel:G,startTime:re,expirationTime:ie,sortIndex:-1},re>O?(G.sortIndex=re,e(d,G),t(h)===null&&G===t(d)&&(E?(x(z),z=-1):E=!0,ce(P,re-O))):(G.sortIndex=ie,e(h,G),M||S||(M=!0,te(H))),G},s.unstable_shouldYield=U,s.unstable_wrapCallback=function(G){var he=g;return function(){var re=g;g=he;try{return G.apply(this,arguments)}finally{g=re}}}})(xf)),xf}var ng;function zy(){return ng||(ng=1,yf.exports=By()),yf.exports}/**
 * @license React
 * react-dom.production.min.js
 *
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */var ig;function ky(){if(ig)return Un;ig=1;var s=fd(),e=zy();function t(n){for(var r="https://reactjs.org/docs/error-decoder.html?invariant="+n,l=1;l<arguments.length;l++)r+="&args[]="+encodeURIComponent(arguments[l]);return"Minified React error #"+n+"; visit "+r+" for the full message or use the non-minified dev environment for full errors and additional helpful warnings."}var i=new Set,o={};function a(n,r){u(n,r),u(n+"Capture",r)}function u(n,r){for(o[n]=r,n=0;n<r.length;n++)i.add(r[n])}var c=!(typeof window>"u"||typeof window.document>"u"||typeof window.document.createElement>"u"),h=Object.prototype.hasOwnProperty,d=/^[:A-Z_a-z\u00C0-\u00D6\u00D8-\u00F6\u00F8-\u02FF\u0370-\u037D\u037F-\u1FFF\u200C-\u200D\u2070-\u218F\u2C00-\u2FEF\u3001-\uD7FF\uF900-\uFDCF\uFDF0-\uFFFD][:A-Z_a-z\u00C0-\u00D6\u00D8-\u00F6\u00F8-\u02FF\u0370-\u037D\u037F-\u1FFF\u200C-\u200D\u2070-\u218F\u2C00-\u2FEF\u3001-\uD7FF\uF900-\uFDCF\uFDF0-\uFFFD\-.0-9\u00B7\u0300-\u036F\u203F-\u2040]*$/,m={},_={};function g(n){return h.call(_,n)?!0:h.call(m,n)?!1:d.test(n)?_[n]=!0:(m[n]=!0,!1)}function S(n,r,l,f){if(l!==null&&l.type===0)return!1;switch(typeof r){case"function":case"symbol":return!0;case"boolean":return f?!1:l!==null?!l.acceptsBooleans:(n=n.toLowerCase().slice(0,5),n!=="data-"&&n!=="aria-");default:return!1}}function M(n,r,l,f){if(r===null||typeof r>"u"||S(n,r,l,f))return!0;if(f)return!1;if(l!==null)switch(l.type){case 3:return!r;case 4:return r===!1;case 5:return isNaN(r);case 6:return isNaN(r)||1>r}return!1}function E(n,r,l,f,p,v,A){this.acceptsBooleans=r===2||r===3||r===4,this.attributeName=f,this.attributeNamespace=p,this.mustUseProperty=l,this.propertyName=n,this.type=r,this.sanitizeURL=v,this.removeEmptyString=A}var y={};"children dangerouslySetInnerHTML defaultValue defaultChecked innerHTML suppressContentEditableWarning suppressHydrationWarning style".split(" ").forEach(function(n){y[n]=new E(n,0,!1,n,null,!1,!1)}),[["acceptCharset","accept-charset"],["className","class"],["htmlFor","for"],["httpEquiv","http-equiv"]].forEach(function(n){var r=n[0];y[r]=new E(r,1,!1,n[1],null,!1,!1)}),["contentEditable","draggable","spellCheck","value"].forEach(function(n){y[n]=new E(n,2,!1,n.toLowerCase(),null,!1,!1)}),["autoReverse","externalResourcesRequired","focusable","preserveAlpha"].forEach(function(n){y[n]=new E(n,2,!1,n,null,!1,!1)}),"allowFullScreen async autoFocus autoPlay controls default defer disabled disablePictureInPicture disableRemotePlayback formNoValidate hidden loop noModule noValidate open playsInline readOnly required reversed scoped seamless itemScope".split(" ").forEach(function(n){y[n]=new E(n,3,!1,n.toLowerCase(),null,!1,!1)}),["checked","multiple","muted","selected"].forEach(function(n){y[n]=new E(n,3,!0,n,null,!1,!1)}),["capture","download"].forEach(function(n){y[n]=new E(n,4,!1,n,null,!1,!1)}),["cols","rows","size","span"].forEach(function(n){y[n]=new E(n,6,!1,n,null,!1,!1)}),["rowSpan","start"].forEach(function(n){y[n]=new E(n,5,!1,n.toLowerCase(),null,!1,!1)});var x=/[\-:]([a-z])/g;function b(n){return n[1].toUpperCase()}"accent-height alignment-baseline arabic-form baseline-shift cap-height clip-path clip-rule color-interpolation color-interpolation-filters color-profile color-rendering dominant-baseline enable-background fill-opacity fill-rule flood-color flood-opacity font-family font-size font-size-adjust font-stretch font-style font-variant font-weight glyph-name glyph-orientation-horizontal glyph-orientation-vertical horiz-adv-x horiz-origin-x image-rendering letter-spacing lighting-color marker-end marker-mid marker-start overline-position overline-thickness paint-order panose-1 pointer-events rendering-intent shape-rendering stop-color stop-opacity strikethrough-position strikethrough-thickness stroke-dasharray stroke-dashoffset stroke-linecap stroke-linejoin stroke-miterlimit stroke-opacity stroke-width text-anchor text-decoration text-rendering underline-position underline-thickness unicode-bidi unicode-range units-per-em v-alphabetic v-hanging v-ideographic v-mathematical vector-effect vert-adv-y vert-origin-x vert-origin-y word-spacing writing-mode xmlns:xlink x-height".split(" ").forEach(function(n){var r=n.replace(x,b);y[r]=new E(r,1,!1,n,null,!1,!1)}),"xlink:actuate xlink:arcrole xlink:role xlink:show xlink:title xlink:type".split(" ").forEach(function(n){var r=n.replace(x,b);y[r]=new E(r,1,!1,n,"http://www.w3.org/1999/xlink",!1,!1)}),["xml:base","xml:lang","xml:space"].forEach(function(n){var r=n.replace(x,b);y[r]=new E(r,1,!1,n,"http://www.w3.org/XML/1998/namespace",!1,!1)}),["tabIndex","crossOrigin"].forEach(function(n){y[n]=new E(n,1,!1,n.toLowerCase(),null,!1,!1)}),y.xlinkHref=new E("xlinkHref",1,!1,"xlink:href","http://www.w3.org/1999/xlink",!0,!1),["src","href","action","formAction"].forEach(function(n){y[n]=new E(n,1,!1,n.toLowerCase(),null,!0,!0)});function T(n,r,l,f){var p=y.hasOwnProperty(r)?y[r]:null;(p!==null?p.type!==0:f||!(2<r.length)||r[0]!=="o"&&r[0]!=="O"||r[1]!=="n"&&r[1]!=="N")&&(M(r,l,p,f)&&(l=null),f||p===null?g(r)&&(l===null?n.removeAttribute(r):n.setAttribute(r,""+l)):p.mustUseProperty?n[p.propertyName]=l===null?p.type===3?!1:"":l:(r=p.attributeName,f=p.attributeNamespace,l===null?n.removeAttribute(r):(p=p.type,l=p===3||p===4&&l===!0?"":""+l,f?n.setAttributeNS(f,r,l):n.setAttribute(r,l))))}var P=s.__SECRET_INTERNALS_DO_NOT_USE_OR_YOU_WILL_BE_FIRED,H=Symbol.for("react.element"),D=Symbol.for("react.portal"),N=Symbol.for("react.fragment"),z=Symbol.for("react.strict_mode"),C=Symbol.for("react.profiler"),w=Symbol.for("react.provider"),U=Symbol.for("react.context"),X=Symbol.for("react.forward_ref"),k=Symbol.for("react.suspense"),Y=Symbol.for("react.suspense_list"),ae=Symbol.for("react.memo"),te=Symbol.for("react.lazy"),ce=Symbol.for("react.offscreen"),G=Symbol.iterator;function he(n){return n===null||typeof n!="object"?null:(n=G&&n[G]||n["@@iterator"],typeof n=="function"?n:null)}var re=Object.assign,O;function ie(n){if(O===void 0)try{throw Error()}catch(l){var r=l.stack.trim().match(/\n( *(at )?)/);O=r&&r[1]||""}return`
`+O+n}var Ae=!1;function Z(n,r){if(!n||Ae)return"";Ae=!0;var l=Error.prepareStackTrace;Error.prepareStackTrace=void 0;try{if(r)if(r=function(){throw Error()},Object.defineProperty(r.prototype,"props",{set:function(){throw Error()}}),typeof Reflect=="object"&&Reflect.construct){try{Reflect.construct(r,[])}catch(ue){var f=ue}Reflect.construct(n,[],r)}else{try{r.call()}catch(ue){f=ue}n.call(r.prototype)}else{try{throw Error()}catch(ue){f=ue}n()}}catch(ue){if(ue&&f&&typeof ue.stack=="string"){for(var p=ue.stack.split(`
`),v=f.stack.split(`
`),A=p.length-1,B=v.length-1;1<=A&&0<=B&&p[A]!==v[B];)B--;for(;1<=A&&0<=B;A--,B--)if(p[A]!==v[B]){if(A!==1||B!==1)do if(A--,B--,0>B||p[A]!==v[B]){var V=`
`+p[A].replace(" at new "," at ");return n.displayName&&V.includes("<anonymous>")&&(V=V.replace("<anonymous>",n.displayName)),V}while(1<=A&&0<=B);break}}}finally{Ae=!1,Error.prepareStackTrace=l}return(n=n?n.displayName||n.name:"")?ie(n):""}function ne(n){switch(n.tag){case 5:return ie(n.type);case 16:return ie("Lazy");case 13:return ie("Suspense");case 19:return ie("SuspenseList");case 0:case 2:case 15:return n=Z(n.type,!1),n;case 11:return n=Z(n.type.render,!1),n;case 1:return n=Z(n.type,!0),n;default:return""}}function xe(n){if(n==null)return null;if(typeof n=="function")return n.displayName||n.name||null;if(typeof n=="string")return n;switch(n){case N:return"Fragment";case D:return"Portal";case C:return"Profiler";case z:return"StrictMode";case k:return"Suspense";case Y:return"SuspenseList"}if(typeof n=="object")switch(n.$$typeof){case U:return(n.displayName||"Context")+".Consumer";case w:return(n._context.displayName||"Context")+".Provider";case X:var r=n.render;return n=n.displayName,n||(n=r.displayName||r.name||"",n=n!==""?"ForwardRef("+n+")":"ForwardRef"),n;case ae:return r=n.displayName||null,r!==null?r:xe(n.type)||"Memo";case te:r=n._payload,n=n._init;try{return xe(n(r))}catch{}}return null}function de(n){var r=n.type;switch(n.tag){case 24:return"Cache";case 9:return(r.displayName||"Context")+".Consumer";case 10:return(r._context.displayName||"Context")+".Provider";case 18:return"DehydratedFragment";case 11:return n=r.render,n=n.displayName||n.name||"",r.displayName||(n!==""?"ForwardRef("+n+")":"ForwardRef");case 7:return"Fragment";case 5:return r;case 4:return"Portal";case 3:return"Root";case 6:return"Text";case 16:return xe(r);case 8:return r===z?"StrictMode":"Mode";case 22:return"Offscreen";case 12:return"Profiler";case 21:return"Scope";case 13:return"Suspense";case 19:return"SuspenseList";case 25:return"TracingMarker";case 1:case 0:case 17:case 2:case 14:case 15:if(typeof r=="function")return r.displayName||r.name||null;if(typeof r=="string")return r}return null}function ee(n){switch(typeof n){case"boolean":case"number":case"string":case"undefined":return n;case"object":return n;default:return""}}function we(n){var r=n.type;return(n=n.nodeName)&&n.toLowerCase()==="input"&&(r==="checkbox"||r==="radio")}function Ue(n){var r=we(n)?"checked":"value",l=Object.getOwnPropertyDescriptor(n.constructor.prototype,r),f=""+n[r];if(!n.hasOwnProperty(r)&&typeof l<"u"&&typeof l.get=="function"&&typeof l.set=="function"){var p=l.get,v=l.set;return Object.defineProperty(n,r,{configurable:!0,get:function(){return p.call(this)},set:function(A){f=""+A,v.call(this,A)}}),Object.defineProperty(n,r,{enumerable:l.enumerable}),{getValue:function(){return f},setValue:function(A){f=""+A},stopTracking:function(){n._valueTracker=null,delete n[r]}}}}function tt(n){n._valueTracker||(n._valueTracker=Ue(n))}function I(n){if(!n)return!1;var r=n._valueTracker;if(!r)return!0;var l=r.getValue(),f="";return n&&(f=we(n)?n.checked?"true":"false":n.value),n=f,n!==l?(r.setValue(n),!0):!1}function Te(n){if(n=n||(typeof document<"u"?document:void 0),typeof n>"u")return null;try{return n.activeElement||n.body}catch{return n.body}}function Se(n,r){var l=r.checked;return re({},r,{defaultChecked:void 0,defaultValue:void 0,value:void 0,checked:l??n._wrapperState.initialChecked})}function Oe(n,r){var l=r.defaultValue==null?"":r.defaultValue,f=r.checked!=null?r.checked:r.defaultChecked;l=ee(r.value!=null?r.value:l),n._wrapperState={initialChecked:f,initialValue:l,controlled:r.type==="checkbox"||r.type==="radio"?r.checked!=null:r.value!=null}}function pe(n,r){r=r.checked,r!=null&&T(n,"checked",r,!1)}function et(n,r){pe(n,r);var l=ee(r.value),f=r.type;if(l!=null)f==="number"?(l===0&&n.value===""||n.value!=l)&&(n.value=""+l):n.value!==""+l&&(n.value=""+l);else if(f==="submit"||f==="reset"){n.removeAttribute("value");return}r.hasOwnProperty("value")?We(n,r.type,l):r.hasOwnProperty("defaultValue")&&We(n,r.type,ee(r.defaultValue)),r.checked==null&&r.defaultChecked!=null&&(n.defaultChecked=!!r.defaultChecked)}function Be(n,r,l){if(r.hasOwnProperty("value")||r.hasOwnProperty("defaultValue")){var f=r.type;if(!(f!=="submit"&&f!=="reset"||r.value!==void 0&&r.value!==null))return;r=""+n._wrapperState.initialValue,l||r===n.value||(n.value=r),n.defaultValue=r}l=n.name,l!==""&&(n.name=""),n.defaultChecked=!!n._wrapperState.initialChecked,l!==""&&(n.name=l)}function We(n,r,l){(r!=="number"||Te(n.ownerDocument)!==n)&&(l==null?n.defaultValue=""+n._wrapperState.initialValue:n.defaultValue!==""+l&&(n.defaultValue=""+l))}var F=Array.isArray;function R(n,r,l,f){if(n=n.options,r){r={};for(var p=0;p<l.length;p++)r["$"+l[p]]=!0;for(l=0;l<n.length;l++)p=r.hasOwnProperty("$"+n[l].value),n[l].selected!==p&&(n[l].selected=p),p&&f&&(n[l].defaultSelected=!0)}else{for(l=""+ee(l),r=null,p=0;p<n.length;p++){if(n[p].value===l){n[p].selected=!0,f&&(n[p].defaultSelected=!0);return}r!==null||n[p].disabled||(r=n[p])}r!==null&&(r.selected=!0)}}function oe(n,r){if(r.dangerouslySetInnerHTML!=null)throw Error(t(91));return re({},r,{value:void 0,defaultValue:void 0,children:""+n._wrapperState.initialValue})}function ve(n,r){var l=r.value;if(l==null){if(l=r.children,r=r.defaultValue,l!=null){if(r!=null)throw Error(t(92));if(F(l)){if(1<l.length)throw Error(t(93));l=l[0]}r=l}r==null&&(r=""),l=r}n._wrapperState={initialValue:ee(l)}}function _e(n,r){var l=ee(r.value),f=ee(r.defaultValue);l!=null&&(l=""+l,l!==n.value&&(n.value=l),r.defaultValue==null&&n.defaultValue!==l&&(n.defaultValue=l)),f!=null&&(n.defaultValue=""+f)}function ye(n){var r=n.textContent;r===n._wrapperState.initialValue&&r!==""&&r!==null&&(n.value=r)}function Qe(n){switch(n){case"svg":return"http://www.w3.org/2000/svg";case"math":return"http://www.w3.org/1998/Math/MathML";default:return"http://www.w3.org/1999/xhtml"}}function Fe(n,r){return n==null||n==="http://www.w3.org/1999/xhtml"?Qe(r):n==="http://www.w3.org/2000/svg"&&r==="foreignObject"?"http://www.w3.org/1999/xhtml":n}var He,ut=(function(n){return typeof MSApp<"u"&&MSApp.execUnsafeLocalFunction?function(r,l,f,p){MSApp.execUnsafeLocalFunction(function(){return n(r,l,f,p)})}:n})(function(n,r){if(n.namespaceURI!=="http://www.w3.org/2000/svg"||"innerHTML"in n)n.innerHTML=r;else{for(He=He||document.createElement("div"),He.innerHTML="<svg>"+r.valueOf().toString()+"</svg>",r=He.firstChild;n.firstChild;)n.removeChild(n.firstChild);for(;r.firstChild;)n.appendChild(r.firstChild)}});function Re(n,r){if(r){var l=n.firstChild;if(l&&l===n.lastChild&&l.nodeType===3){l.nodeValue=r;return}}n.textContent=r}var ke={animationIterationCount:!0,aspectRatio:!0,borderImageOutset:!0,borderImageSlice:!0,borderImageWidth:!0,boxFlex:!0,boxFlexGroup:!0,boxOrdinalGroup:!0,columnCount:!0,columns:!0,flex:!0,flexGrow:!0,flexPositive:!0,flexShrink:!0,flexNegative:!0,flexOrder:!0,gridArea:!0,gridRow:!0,gridRowEnd:!0,gridRowSpan:!0,gridRowStart:!0,gridColumn:!0,gridColumnEnd:!0,gridColumnSpan:!0,gridColumnStart:!0,fontWeight:!0,lineClamp:!0,lineHeight:!0,opacity:!0,order:!0,orphans:!0,tabSize:!0,widows:!0,zIndex:!0,zoom:!0,fillOpacity:!0,floodOpacity:!0,stopOpacity:!0,strokeDasharray:!0,strokeDashoffset:!0,strokeMiterlimit:!0,strokeOpacity:!0,strokeWidth:!0},_t=["Webkit","ms","Moz","O"];Object.keys(ke).forEach(function(n){_t.forEach(function(r){r=r+n.charAt(0).toUpperCase()+n.substring(1),ke[r]=ke[n]})});function at(n,r,l){return r==null||typeof r=="boolean"||r===""?"":l||typeof r!="number"||r===0||ke.hasOwnProperty(n)&&ke[n]?(""+r).trim():r+"px"}function Xe(n,r){n=n.style;for(var l in r)if(r.hasOwnProperty(l)){var f=l.indexOf("--")===0,p=at(l,r[l],f);l==="float"&&(l="cssFloat"),f?n.setProperty(l,p):n[l]=p}}var ct=re({menuitem:!0},{area:!0,base:!0,br:!0,col:!0,embed:!0,hr:!0,img:!0,input:!0,keygen:!0,link:!0,meta:!0,param:!0,source:!0,track:!0,wbr:!0});function ft(n,r){if(r){if(ct[n]&&(r.children!=null||r.dangerouslySetInnerHTML!=null))throw Error(t(137,n));if(r.dangerouslySetInnerHTML!=null){if(r.children!=null)throw Error(t(60));if(typeof r.dangerouslySetInnerHTML!="object"||!("__html"in r.dangerouslySetInnerHTML))throw Error(t(61))}if(r.style!=null&&typeof r.style!="object")throw Error(t(62))}}function bt(n,r){if(n.indexOf("-")===-1)return typeof r.is=="string";switch(n){case"annotation-xml":case"color-profile":case"font-face":case"font-face-src":case"font-face-uri":case"font-face-format":case"font-face-name":case"missing-glyph":return!1;default:return!0}}var q=null;function Le(n){return n=n.target||n.srcElement||window,n.correspondingUseElement&&(n=n.correspondingUseElement),n.nodeType===3?n.parentNode:n}var me=null,ge=null,Ne=null;function rt(n){if(n=Go(n)){if(typeof me!="function")throw Error(t(280));var r=n.stateNode;r&&(r=Ka(r),me(n.stateNode,n.type,r))}}function vt(n){ge?Ne?Ne.push(n):Ne=[n]:ge=n}function Ht(){if(ge){var n=ge,r=Ne;if(Ne=ge=null,rt(n),r)for(n=0;n<r.length;n++)rt(r[n])}}function qt(n,r){return n(r)}function St(){}var zn=!1;function kn(n,r,l){if(zn)return n(r,l);zn=!0;try{return qt(n,r,l)}finally{zn=!1,(ge!==null||Ne!==null)&&(St(),Ht())}}function $i(n,r){var l=n.stateNode;if(l===null)return null;var f=Ka(l);if(f===null)return null;l=f[r];e:switch(r){case"onClick":case"onClickCapture":case"onDoubleClick":case"onDoubleClickCapture":case"onMouseDown":case"onMouseDownCapture":case"onMouseMove":case"onMouseMoveCapture":case"onMouseUp":case"onMouseUpCapture":case"onMouseEnter":(f=!f.disabled)||(n=n.type,f=!(n==="button"||n==="input"||n==="select"||n==="textarea")),n=!f;break e;default:n=!1}if(n)return null;if(l&&typeof l!="function")throw Error(t(231,r,typeof l));return l}var si=!1;if(c)try{var Ri={};Object.defineProperty(Ri,"passive",{get:function(){si=!0}}),window.addEventListener("test",Ri,Ri),window.removeEventListener("test",Ri,Ri)}catch{si=!1}function Pa(n,r,l,f,p,v,A,B,V){var ue=Array.prototype.slice.call(arguments,3);try{r.apply(l,ue)}catch(Ee){this.onError(Ee)}}var Zi=!1,Pi=null,Ur=!1,Ji=null,ba={onError:function(n){Zi=!0,Pi=n}};function La(n,r,l,f,p,v,A,B,V){Zi=!1,Pi=null,Pa.apply(ba,arguments)}function Ou(n,r,l,f,p,v,A,B,V){if(La.apply(this,arguments),Zi){if(Zi){var ue=Pi;Zi=!1,Pi=null}else throw Error(t(198));Ur||(Ur=!0,Ji=ue)}}function bi(n){var r=n,l=n;if(n.alternate)for(;r.return;)r=r.return;else{n=r;do r=n,(r.flags&4098)!==0&&(l=r.return),n=r.return;while(n)}return r.tag===3?l:null}function Da(n){if(n.tag===13){var r=n.memoizedState;if(r===null&&(n=n.alternate,n!==null&&(r=n.memoizedState)),r!==null)return r.dehydrated}return null}function L(n){if(bi(n)!==n)throw Error(t(188))}function K(n){var r=n.alternate;if(!r){if(r=bi(n),r===null)throw Error(t(188));return r!==n?null:n}for(var l=n,f=r;;){var p=l.return;if(p===null)break;var v=p.alternate;if(v===null){if(f=p.return,f!==null){l=f;continue}break}if(p.child===v.child){for(v=p.child;v;){if(v===l)return L(p),n;if(v===f)return L(p),r;v=v.sibling}throw Error(t(188))}if(l.return!==f.return)l=p,f=v;else{for(var A=!1,B=p.child;B;){if(B===l){A=!0,l=p,f=v;break}if(B===f){A=!0,f=p,l=v;break}B=B.sibling}if(!A){for(B=v.child;B;){if(B===l){A=!0,l=v,f=p;break}if(B===f){A=!0,f=v,l=p;break}B=B.sibling}if(!A)throw Error(t(189))}}if(l.alternate!==f)throw Error(t(190))}if(l.tag!==3)throw Error(t(188));return l.stateNode.current===l?n:r}function se(n){return n=K(n),n!==null?le(n):null}function le(n){if(n.tag===5||n.tag===6)return n;for(n=n.child;n!==null;){var r=le(n);if(r!==null)return r;n=n.sibling}return null}var $=e.unstable_scheduleCallback,De=e.unstable_cancelCallback,Ve=e.unstable_shouldYield,Ye=e.unstable_requestPaint,Ie=e.unstable_now,ot=e.unstable_getCurrentPriorityLevel,st=e.unstable_ImmediatePriority,Ze=e.unstable_UserBlockingPriority,xt=e.unstable_NormalPriority,Ot=e.unstable_LowPriority,Nt=e.unstable_IdlePriority,nn=null,ht=null;function Je(n){if(ht&&typeof ht.onCommitFiberRoot=="function")try{ht.onCommitFiberRoot(nn,n,void 0,(n.current.flags&128)===128)}catch{}}var At=Math.clz32?Math.clz32:Qi,Et=Math.log,Hn=Math.LN2;function Qi(n){return n>>>=0,n===0?32:31-(Et(n)/Hn|0)|0}var rn=64,er=4194304;function Lt(n){switch(n&-n){case 1:return 1;case 2:return 2;case 4:return 4;case 8:return 8;case 16:return 16;case 32:return 32;case 64:case 128:case 256:case 512:case 1024:case 2048:case 4096:case 8192:case 16384:case 32768:case 65536:case 131072:case 262144:case 524288:case 1048576:case 2097152:return n&4194240;case 4194304:case 8388608:case 16777216:case 33554432:case 67108864:return n&130023424;case 134217728:return 134217728;case 268435456:return 268435456;case 536870912:return 536870912;case 1073741824:return 1073741824;default:return n}}function Vn(n,r){var l=n.pendingLanes;if(l===0)return 0;var f=0,p=n.suspendedLanes,v=n.pingedLanes,A=l&268435455;if(A!==0){var B=A&~p;B!==0?f=Lt(B):(v&=A,v!==0&&(f=Lt(v)))}else A=l&~p,A!==0?f=Lt(A):v!==0&&(f=Lt(v));if(f===0)return 0;if(r!==0&&r!==f&&(r&p)===0&&(p=f&-f,v=r&-r,p>=v||p===16&&(v&4194240)!==0))return r;if((f&4)!==0&&(f|=l&16),r=n.entangledLanes,r!==0)for(n=n.entanglements,r&=f;0<r;)l=31-At(r),p=1<<l,f|=n[l],r&=~p;return f}function To(n,r){switch(n){case 1:case 2:case 4:return r+250;case 8:case 16:case 32:case 64:case 128:case 256:case 512:case 1024:case 2048:case 4096:case 8192:case 16384:case 32768:case 65536:case 131072:case 262144:case 524288:case 1048576:case 2097152:return r+5e3;case 4194304:case 8388608:case 16777216:case 33554432:case 67108864:return-1;case 134217728:case 268435456:case 536870912:case 1073741824:return-1;default:return-1}}function Pn(n,r){for(var l=n.suspendedLanes,f=n.pingedLanes,p=n.expirationTimes,v=n.pendingLanes;0<v;){var A=31-At(v),B=1<<A,V=p[A];V===-1?((B&l)===0||(B&f)!==0)&&(p[A]=To(B,r)):V<=r&&(n.expiredLanes|=B),v&=~B}}function Fr(n){return n=n.pendingLanes&-1073741825,n!==0?n:n&1073741824?1073741824:0}function Na(){var n=rn;return rn<<=1,(rn&4194240)===0&&(rn=64),n}function ps(n){for(var r=[],l=0;31>l;l++)r.push(n);return r}function Ao(n,r,l){n.pendingLanes|=r,r!==536870912&&(n.suspendedLanes=0,n.pingedLanes=0),n=n.eventTimes,r=31-At(r),n[r]=l}function tv(n,r){var l=n.pendingLanes&~r;n.pendingLanes=r,n.suspendedLanes=0,n.pingedLanes=0,n.expiredLanes&=r,n.mutableReadLanes&=r,n.entangledLanes&=r,r=n.entanglements;var f=n.eventTimes;for(n=n.expirationTimes;0<l;){var p=31-At(l),v=1<<p;r[p]=0,f[p]=-1,n[p]=-1,l&=~v}}function Bu(n,r){var l=n.entangledLanes|=r;for(n=n.entanglements;l;){var f=31-At(l),p=1<<f;p&r|n[f]&r&&(n[f]|=r),l&=~p}}var Ct=0;function Pd(n){return n&=-n,1<n?4<n?(n&268435455)!==0?16:536870912:4:1}var bd,zu,Ld,Dd,Nd,ku=!1,Ia=[],tr=null,nr=null,ir=null,Co=new Map,Ro=new Map,rr=[],nv="mousedown mouseup touchcancel touchend touchstart auxclick dblclick pointercancel pointerdown pointerup dragend dragstart drop compositionend compositionstart keydown keypress keyup input textInput copy cut paste click change contextmenu reset submit".split(" ");function Id(n,r){switch(n){case"focusin":case"focusout":tr=null;break;case"dragenter":case"dragleave":nr=null;break;case"mouseover":case"mouseout":ir=null;break;case"pointerover":case"pointerout":Co.delete(r.pointerId);break;case"gotpointercapture":case"lostpointercapture":Ro.delete(r.pointerId)}}function Po(n,r,l,f,p,v){return n===null||n.nativeEvent!==v?(n={blockedOn:r,domEventName:l,eventSystemFlags:f,nativeEvent:v,targetContainers:[p]},r!==null&&(r=Go(r),r!==null&&zu(r)),n):(n.eventSystemFlags|=f,r=n.targetContainers,p!==null&&r.indexOf(p)===-1&&r.push(p),n)}function iv(n,r,l,f,p){switch(r){case"focusin":return tr=Po(tr,n,r,l,f,p),!0;case"dragenter":return nr=Po(nr,n,r,l,f,p),!0;case"mouseover":return ir=Po(ir,n,r,l,f,p),!0;case"pointerover":var v=p.pointerId;return Co.set(v,Po(Co.get(v)||null,n,r,l,f,p)),!0;case"gotpointercapture":return v=p.pointerId,Ro.set(v,Po(Ro.get(v)||null,n,r,l,f,p)),!0}return!1}function Ud(n){var r=Or(n.target);if(r!==null){var l=bi(r);if(l!==null){if(r=l.tag,r===13){if(r=Da(l),r!==null){n.blockedOn=r,Nd(n.priority,function(){Ld(l)});return}}else if(r===3&&l.stateNode.current.memoizedState.isDehydrated){n.blockedOn=l.tag===3?l.stateNode.containerInfo:null;return}}}n.blockedOn=null}function Ua(n){if(n.blockedOn!==null)return!1;for(var r=n.targetContainers;0<r.length;){var l=Vu(n.domEventName,n.eventSystemFlags,r[0],n.nativeEvent);if(l===null){l=n.nativeEvent;var f=new l.constructor(l.type,l);q=f,l.target.dispatchEvent(f),q=null}else return r=Go(l),r!==null&&zu(r),n.blockedOn=l,!1;r.shift()}return!0}function Fd(n,r,l){Ua(n)&&l.delete(r)}function rv(){ku=!1,tr!==null&&Ua(tr)&&(tr=null),nr!==null&&Ua(nr)&&(nr=null),ir!==null&&Ua(ir)&&(ir=null),Co.forEach(Fd),Ro.forEach(Fd)}function bo(n,r){n.blockedOn===r&&(n.blockedOn=null,ku||(ku=!0,e.unstable_scheduleCallback(e.unstable_NormalPriority,rv)))}function Lo(n){function r(p){return bo(p,n)}if(0<Ia.length){bo(Ia[0],n);for(var l=1;l<Ia.length;l++){var f=Ia[l];f.blockedOn===n&&(f.blockedOn=null)}}for(tr!==null&&bo(tr,n),nr!==null&&bo(nr,n),ir!==null&&bo(ir,n),Co.forEach(r),Ro.forEach(r),l=0;l<rr.length;l++)f=rr[l],f.blockedOn===n&&(f.blockedOn=null);for(;0<rr.length&&(l=rr[0],l.blockedOn===null);)Ud(l),l.blockedOn===null&&rr.shift()}var ms=P.ReactCurrentBatchConfig,Fa=!0;function sv(n,r,l,f){var p=Ct,v=ms.transition;ms.transition=null;try{Ct=1,Hu(n,r,l,f)}finally{Ct=p,ms.transition=v}}function ov(n,r,l,f){var p=Ct,v=ms.transition;ms.transition=null;try{Ct=4,Hu(n,r,l,f)}finally{Ct=p,ms.transition=v}}function Hu(n,r,l,f){if(Fa){var p=Vu(n,r,l,f);if(p===null)sc(n,r,f,Oa,l),Id(n,f);else if(iv(p,n,r,l,f))f.stopPropagation();else if(Id(n,f),r&4&&-1<nv.indexOf(n)){for(;p!==null;){var v=Go(p);if(v!==null&&bd(v),v=Vu(n,r,l,f),v===null&&sc(n,r,f,Oa,l),v===p)break;p=v}p!==null&&f.stopPropagation()}else sc(n,r,f,null,l)}}var Oa=null;function Vu(n,r,l,f){if(Oa=null,n=Le(f),n=Or(n),n!==null)if(r=bi(n),r===null)n=null;else if(l=r.tag,l===13){if(n=Da(r),n!==null)return n;n=null}else if(l===3){if(r.stateNode.current.memoizedState.isDehydrated)return r.tag===3?r.stateNode.containerInfo:null;n=null}else r!==n&&(n=null);return Oa=n,null}function Od(n){switch(n){case"cancel":case"click":case"close":case"contextmenu":case"copy":case"cut":case"auxclick":case"dblclick":case"dragend":case"dragstart":case"drop":case"focusin":case"focusout":case"input":case"invalid":case"keydown":case"keypress":case"keyup":case"mousedown":case"mouseup":case"paste":case"pause":case"play":case"pointercancel":case"pointerdown":case"pointerup":case"ratechange":case"reset":case"resize":case"seeked":case"submit":case"touchcancel":case"touchend":case"touchstart":case"volumechange":case"change":case"selectionchange":case"textInput":case"compositionstart":case"compositionend":case"compositionupdate":case"beforeblur":case"afterblur":case"beforeinput":case"blur":case"fullscreenchange":case"focus":case"hashchange":case"popstate":case"select":case"selectstart":return 1;case"drag":case"dragenter":case"dragexit":case"dragleave":case"dragover":case"mousemove":case"mouseout":case"mouseover":case"pointermove":case"pointerout":case"pointerover":case"scroll":case"toggle":case"touchmove":case"wheel":case"mouseenter":case"mouseleave":case"pointerenter":case"pointerleave":return 4;case"message":switch(ot()){case st:return 1;case Ze:return 4;case xt:case Ot:return 16;case Nt:return 536870912;default:return 16}default:return 16}}var sr=null,Gu=null,Ba=null;function Bd(){if(Ba)return Ba;var n,r=Gu,l=r.length,f,p="value"in sr?sr.value:sr.textContent,v=p.length;for(n=0;n<l&&r[n]===p[n];n++);var A=l-n;for(f=1;f<=A&&r[l-f]===p[v-f];f++);return Ba=p.slice(n,1<f?1-f:void 0)}function za(n){var r=n.keyCode;return"charCode"in n?(n=n.charCode,n===0&&r===13&&(n=13)):n=r,n===10&&(n=13),32<=n||n===13?n:0}function ka(){return!0}function zd(){return!1}function Gn(n){function r(l,f,p,v,A){this._reactName=l,this._targetInst=p,this.type=f,this.nativeEvent=v,this.target=A,this.currentTarget=null;for(var B in n)n.hasOwnProperty(B)&&(l=n[B],this[B]=l?l(v):v[B]);return this.isDefaultPrevented=(v.defaultPrevented!=null?v.defaultPrevented:v.returnValue===!1)?ka:zd,this.isPropagationStopped=zd,this}return re(r.prototype,{preventDefault:function(){this.defaultPrevented=!0;var l=this.nativeEvent;l&&(l.preventDefault?l.preventDefault():typeof l.returnValue!="unknown"&&(l.returnValue=!1),this.isDefaultPrevented=ka)},stopPropagation:function(){var l=this.nativeEvent;l&&(l.stopPropagation?l.stopPropagation():typeof l.cancelBubble!="unknown"&&(l.cancelBubble=!0),this.isPropagationStopped=ka)},persist:function(){},isPersistent:ka}),r}var gs={eventPhase:0,bubbles:0,cancelable:0,timeStamp:function(n){return n.timeStamp||Date.now()},defaultPrevented:0,isTrusted:0},Wu=Gn(gs),Do=re({},gs,{view:0,detail:0}),av=Gn(Do),Xu,ju,No,Ha=re({},Do,{screenX:0,screenY:0,clientX:0,clientY:0,pageX:0,pageY:0,ctrlKey:0,shiftKey:0,altKey:0,metaKey:0,getModifierState:Yu,button:0,buttons:0,relatedTarget:function(n){return n.relatedTarget===void 0?n.fromElement===n.srcElement?n.toElement:n.fromElement:n.relatedTarget},movementX:function(n){return"movementX"in n?n.movementX:(n!==No&&(No&&n.type==="mousemove"?(Xu=n.screenX-No.screenX,ju=n.screenY-No.screenY):ju=Xu=0,No=n),Xu)},movementY:function(n){return"movementY"in n?n.movementY:ju}}),kd=Gn(Ha),lv=re({},Ha,{dataTransfer:0}),uv=Gn(lv),cv=re({},Do,{relatedTarget:0}),qu=Gn(cv),fv=re({},gs,{animationName:0,elapsedTime:0,pseudoElement:0}),hv=Gn(fv),dv=re({},gs,{clipboardData:function(n){return"clipboardData"in n?n.clipboardData:window.clipboardData}}),pv=Gn(dv),mv=re({},gs,{data:0}),Hd=Gn(mv),gv={Esc:"Escape",Spacebar:" ",Left:"ArrowLeft",Up:"ArrowUp",Right:"ArrowRight",Down:"ArrowDown",Del:"Delete",Win:"OS",Menu:"ContextMenu",Apps:"ContextMenu",Scroll:"ScrollLock",MozPrintableKey:"Unidentified"},_v={8:"Backspace",9:"Tab",12:"Clear",13:"Enter",16:"Shift",17:"Control",18:"Alt",19:"Pause",20:"CapsLock",27:"Escape",32:" ",33:"PageUp",34:"PageDown",35:"End",36:"Home",37:"ArrowLeft",38:"ArrowUp",39:"ArrowRight",40:"ArrowDown",45:"Insert",46:"Delete",112:"F1",113:"F2",114:"F3",115:"F4",116:"F5",117:"F6",118:"F7",119:"F8",120:"F9",121:"F10",122:"F11",123:"F12",144:"NumLock",145:"ScrollLock",224:"Meta"},vv={Alt:"altKey",Control:"ctrlKey",Meta:"metaKey",Shift:"shiftKey"};function yv(n){var r=this.nativeEvent;return r.getModifierState?r.getModifierState(n):(n=vv[n])?!!r[n]:!1}function Yu(){return yv}var xv=re({},Do,{key:function(n){if(n.key){var r=gv[n.key]||n.key;if(r!=="Unidentified")return r}return n.type==="keypress"?(n=za(n),n===13?"Enter":String.fromCharCode(n)):n.type==="keydown"||n.type==="keyup"?_v[n.keyCode]||"Unidentified":""},code:0,location:0,ctrlKey:0,shiftKey:0,altKey:0,metaKey:0,repeat:0,locale:0,getModifierState:Yu,charCode:function(n){return n.type==="keypress"?za(n):0},keyCode:function(n){return n.type==="keydown"||n.type==="keyup"?n.keyCode:0},which:function(n){return n.type==="keypress"?za(n):n.type==="keydown"||n.type==="keyup"?n.keyCode:0}}),Sv=Gn(xv),Mv=re({},Ha,{pointerId:0,width:0,height:0,pressure:0,tangentialPressure:0,tiltX:0,tiltY:0,twist:0,pointerType:0,isPrimary:0}),Vd=Gn(Mv),Ev=re({},Do,{touches:0,targetTouches:0,changedTouches:0,altKey:0,metaKey:0,ctrlKey:0,shiftKey:0,getModifierState:Yu}),wv=Gn(Ev),Tv=re({},gs,{propertyName:0,elapsedTime:0,pseudoElement:0}),Av=Gn(Tv),Cv=re({},Ha,{deltaX:function(n){return"deltaX"in n?n.deltaX:"wheelDeltaX"in n?-n.wheelDeltaX:0},deltaY:function(n){return"deltaY"in n?n.deltaY:"wheelDeltaY"in n?-n.wheelDeltaY:"wheelDelta"in n?-n.wheelDelta:0},deltaZ:0,deltaMode:0}),Rv=Gn(Cv),Pv=[9,13,27,32],Ku=c&&"CompositionEvent"in window,Io=null;c&&"documentMode"in document&&(Io=document.documentMode);var bv=c&&"TextEvent"in window&&!Io,Gd=c&&(!Ku||Io&&8<Io&&11>=Io),Wd=" ",Xd=!1;function jd(n,r){switch(n){case"keyup":return Pv.indexOf(r.keyCode)!==-1;case"keydown":return r.keyCode!==229;case"keypress":case"mousedown":case"focusout":return!0;default:return!1}}function qd(n){return n=n.detail,typeof n=="object"&&"data"in n?n.data:null}var _s=!1;function Lv(n,r){switch(n){case"compositionend":return qd(r);case"keypress":return r.which!==32?null:(Xd=!0,Wd);case"textInput":return n=r.data,n===Wd&&Xd?null:n;default:return null}}function Dv(n,r){if(_s)return n==="compositionend"||!Ku&&jd(n,r)?(n=Bd(),Ba=Gu=sr=null,_s=!1,n):null;switch(n){case"paste":return null;case"keypress":if(!(r.ctrlKey||r.altKey||r.metaKey)||r.ctrlKey&&r.altKey){if(r.char&&1<r.char.length)return r.char;if(r.which)return String.fromCharCode(r.which)}return null;case"compositionend":return Gd&&r.locale!=="ko"?null:r.data;default:return null}}var Nv={color:!0,date:!0,datetime:!0,"datetime-local":!0,email:!0,month:!0,number:!0,password:!0,range:!0,search:!0,tel:!0,text:!0,time:!0,url:!0,week:!0};function Yd(n){var r=n&&n.nodeName&&n.nodeName.toLowerCase();return r==="input"?!!Nv[n.type]:r==="textarea"}function Kd(n,r,l,f){vt(f),r=ja(r,"onChange"),0<r.length&&(l=new Wu("onChange","change",null,l,f),n.push({event:l,listeners:r}))}var Uo=null,Fo=null;function Iv(n){dp(n,0)}function Va(n){var r=Ms(n);if(I(r))return n}function Uv(n,r){if(n==="change")return r}var $d=!1;if(c){var $u;if(c){var Zu="oninput"in document;if(!Zu){var Zd=document.createElement("div");Zd.setAttribute("oninput","return;"),Zu=typeof Zd.oninput=="function"}$u=Zu}else $u=!1;$d=$u&&(!document.documentMode||9<document.documentMode)}function Jd(){Uo&&(Uo.detachEvent("onpropertychange",Qd),Fo=Uo=null)}function Qd(n){if(n.propertyName==="value"&&Va(Fo)){var r=[];Kd(r,Fo,n,Le(n)),kn(Iv,r)}}function Fv(n,r,l){n==="focusin"?(Jd(),Uo=r,Fo=l,Uo.attachEvent("onpropertychange",Qd)):n==="focusout"&&Jd()}function Ov(n){if(n==="selectionchange"||n==="keyup"||n==="keydown")return Va(Fo)}function Bv(n,r){if(n==="click")return Va(r)}function zv(n,r){if(n==="input"||n==="change")return Va(r)}function kv(n,r){return n===r&&(n!==0||1/n===1/r)||n!==n&&r!==r}var oi=typeof Object.is=="function"?Object.is:kv;function Oo(n,r){if(oi(n,r))return!0;if(typeof n!="object"||n===null||typeof r!="object"||r===null)return!1;var l=Object.keys(n),f=Object.keys(r);if(l.length!==f.length)return!1;for(f=0;f<l.length;f++){var p=l[f];if(!h.call(r,p)||!oi(n[p],r[p]))return!1}return!0}function ep(n){for(;n&&n.firstChild;)n=n.firstChild;return n}function tp(n,r){var l=ep(n);n=0;for(var f;l;){if(l.nodeType===3){if(f=n+l.textContent.length,n<=r&&f>=r)return{node:l,offset:r-n};n=f}e:{for(;l;){if(l.nextSibling){l=l.nextSibling;break e}l=l.parentNode}l=void 0}l=ep(l)}}function np(n,r){return n&&r?n===r?!0:n&&n.nodeType===3?!1:r&&r.nodeType===3?np(n,r.parentNode):"contains"in n?n.contains(r):n.compareDocumentPosition?!!(n.compareDocumentPosition(r)&16):!1:!1}function ip(){for(var n=window,r=Te();r instanceof n.HTMLIFrameElement;){try{var l=typeof r.contentWindow.location.href=="string"}catch{l=!1}if(l)n=r.contentWindow;else break;r=Te(n.document)}return r}function Ju(n){var r=n&&n.nodeName&&n.nodeName.toLowerCase();return r&&(r==="input"&&(n.type==="text"||n.type==="search"||n.type==="tel"||n.type==="url"||n.type==="password")||r==="textarea"||n.contentEditable==="true")}function Hv(n){var r=ip(),l=n.focusedElem,f=n.selectionRange;if(r!==l&&l&&l.ownerDocument&&np(l.ownerDocument.documentElement,l)){if(f!==null&&Ju(l)){if(r=f.start,n=f.end,n===void 0&&(n=r),"selectionStart"in l)l.selectionStart=r,l.selectionEnd=Math.min(n,l.value.length);else if(n=(r=l.ownerDocument||document)&&r.defaultView||window,n.getSelection){n=n.getSelection();var p=l.textContent.length,v=Math.min(f.start,p);f=f.end===void 0?v:Math.min(f.end,p),!n.extend&&v>f&&(p=f,f=v,v=p),p=tp(l,v);var A=tp(l,f);p&&A&&(n.rangeCount!==1||n.anchorNode!==p.node||n.anchorOffset!==p.offset||n.focusNode!==A.node||n.focusOffset!==A.offset)&&(r=r.createRange(),r.setStart(p.node,p.offset),n.removeAllRanges(),v>f?(n.addRange(r),n.extend(A.node,A.offset)):(r.setEnd(A.node,A.offset),n.addRange(r)))}}for(r=[],n=l;n=n.parentNode;)n.nodeType===1&&r.push({element:n,left:n.scrollLeft,top:n.scrollTop});for(typeof l.focus=="function"&&l.focus(),l=0;l<r.length;l++)n=r[l],n.element.scrollLeft=n.left,n.element.scrollTop=n.top}}var Vv=c&&"documentMode"in document&&11>=document.documentMode,vs=null,Qu=null,Bo=null,ec=!1;function rp(n,r,l){var f=l.window===l?l.document:l.nodeType===9?l:l.ownerDocument;ec||vs==null||vs!==Te(f)||(f=vs,"selectionStart"in f&&Ju(f)?f={start:f.selectionStart,end:f.selectionEnd}:(f=(f.ownerDocument&&f.ownerDocument.defaultView||window).getSelection(),f={anchorNode:f.anchorNode,anchorOffset:f.anchorOffset,focusNode:f.focusNode,focusOffset:f.focusOffset}),Bo&&Oo(Bo,f)||(Bo=f,f=ja(Qu,"onSelect"),0<f.length&&(r=new Wu("onSelect","select",null,r,l),n.push({event:r,listeners:f}),r.target=vs)))}function Ga(n,r){var l={};return l[n.toLowerCase()]=r.toLowerCase(),l["Webkit"+n]="webkit"+r,l["Moz"+n]="moz"+r,l}var ys={animationend:Ga("Animation","AnimationEnd"),animationiteration:Ga("Animation","AnimationIteration"),animationstart:Ga("Animation","AnimationStart"),transitionend:Ga("Transition","TransitionEnd")},tc={},sp={};c&&(sp=document.createElement("div").style,"AnimationEvent"in window||(delete ys.animationend.animation,delete ys.animationiteration.animation,delete ys.animationstart.animation),"TransitionEvent"in window||delete ys.transitionend.transition);function Wa(n){if(tc[n])return tc[n];if(!ys[n])return n;var r=ys[n],l;for(l in r)if(r.hasOwnProperty(l)&&l in sp)return tc[n]=r[l];return n}var op=Wa("animationend"),ap=Wa("animationiteration"),lp=Wa("animationstart"),up=Wa("transitionend"),cp=new Map,fp="abort auxClick cancel canPlay canPlayThrough click close contextMenu copy cut drag dragEnd dragEnter dragExit dragLeave dragOver dragStart drop durationChange emptied encrypted ended error gotPointerCapture input invalid keyDown keyPress keyUp load loadedData loadedMetadata loadStart lostPointerCapture mouseDown mouseMove mouseOut mouseOver mouseUp paste pause play playing pointerCancel pointerDown pointerMove pointerOut pointerOver pointerUp progress rateChange reset resize seeked seeking stalled submit suspend timeUpdate touchCancel touchEnd touchStart volumeChange scroll toggle touchMove waiting wheel".split(" ");function or(n,r){cp.set(n,r),a(r,[n])}for(var nc=0;nc<fp.length;nc++){var ic=fp[nc],Gv=ic.toLowerCase(),Wv=ic[0].toUpperCase()+ic.slice(1);or(Gv,"on"+Wv)}or(op,"onAnimationEnd"),or(ap,"onAnimationIteration"),or(lp,"onAnimationStart"),or("dblclick","onDoubleClick"),or("focusin","onFocus"),or("focusout","onBlur"),or(up,"onTransitionEnd"),u("onMouseEnter",["mouseout","mouseover"]),u("onMouseLeave",["mouseout","mouseover"]),u("onPointerEnter",["pointerout","pointerover"]),u("onPointerLeave",["pointerout","pointerover"]),a("onChange","change click focusin focusout input keydown keyup selectionchange".split(" ")),a("onSelect","focusout contextmenu dragend focusin keydown keyup mousedown mouseup selectionchange".split(" ")),a("onBeforeInput",["compositionend","keypress","textInput","paste"]),a("onCompositionEnd","compositionend focusout keydown keypress keyup mousedown".split(" ")),a("onCompositionStart","compositionstart focusout keydown keypress keyup mousedown".split(" ")),a("onCompositionUpdate","compositionupdate focusout keydown keypress keyup mousedown".split(" "));var zo="abort canplay canplaythrough durationchange emptied encrypted ended error loadeddata loadedmetadata loadstart pause play playing progress ratechange resize seeked seeking stalled suspend timeupdate volumechange waiting".split(" "),Xv=new Set("cancel close invalid load scroll toggle".split(" ").concat(zo));function hp(n,r,l){var f=n.type||"unknown-event";n.currentTarget=l,Ou(f,r,void 0,n),n.currentTarget=null}function dp(n,r){r=(r&4)!==0;for(var l=0;l<n.length;l++){var f=n[l],p=f.event;f=f.listeners;e:{var v=void 0;if(r)for(var A=f.length-1;0<=A;A--){var B=f[A],V=B.instance,ue=B.currentTarget;if(B=B.listener,V!==v&&p.isPropagationStopped())break e;hp(p,B,ue),v=V}else for(A=0;A<f.length;A++){if(B=f[A],V=B.instance,ue=B.currentTarget,B=B.listener,V!==v&&p.isPropagationStopped())break e;hp(p,B,ue),v=V}}}if(Ur)throw n=Ji,Ur=!1,Ji=null,n}function It(n,r){var l=r[fc];l===void 0&&(l=r[fc]=new Set);var f=n+"__bubble";l.has(f)||(pp(r,n,2,!1),l.add(f))}function rc(n,r,l){var f=0;r&&(f|=4),pp(l,n,f,r)}var Xa="_reactListening"+Math.random().toString(36).slice(2);function ko(n){if(!n[Xa]){n[Xa]=!0,i.forEach(function(l){l!=="selectionchange"&&(Xv.has(l)||rc(l,!1,n),rc(l,!0,n))});var r=n.nodeType===9?n:n.ownerDocument;r===null||r[Xa]||(r[Xa]=!0,rc("selectionchange",!1,r))}}function pp(n,r,l,f){switch(Od(r)){case 1:var p=sv;break;case 4:p=ov;break;default:p=Hu}l=p.bind(null,r,l,n),p=void 0,!si||r!=="touchstart"&&r!=="touchmove"&&r!=="wheel"||(p=!0),f?p!==void 0?n.addEventListener(r,l,{capture:!0,passive:p}):n.addEventListener(r,l,!0):p!==void 0?n.addEventListener(r,l,{passive:p}):n.addEventListener(r,l,!1)}function sc(n,r,l,f,p){var v=f;if((r&1)===0&&(r&2)===0&&f!==null)e:for(;;){if(f===null)return;var A=f.tag;if(A===3||A===4){var B=f.stateNode.containerInfo;if(B===p||B.nodeType===8&&B.parentNode===p)break;if(A===4)for(A=f.return;A!==null;){var V=A.tag;if((V===3||V===4)&&(V=A.stateNode.containerInfo,V===p||V.nodeType===8&&V.parentNode===p))return;A=A.return}for(;B!==null;){if(A=Or(B),A===null)return;if(V=A.tag,V===5||V===6){f=v=A;continue e}B=B.parentNode}}f=f.return}kn(function(){var ue=v,Ee=Le(l),Ce=[];e:{var Me=cp.get(n);if(Me!==void 0){var Ge=Wu,qe=n;switch(n){case"keypress":if(za(l)===0)break e;case"keydown":case"keyup":Ge=Sv;break;case"focusin":qe="focus",Ge=qu;break;case"focusout":qe="blur",Ge=qu;break;case"beforeblur":case"afterblur":Ge=qu;break;case"click":if(l.button===2)break e;case"auxclick":case"dblclick":case"mousedown":case"mousemove":case"mouseup":case"mouseout":case"mouseover":case"contextmenu":Ge=kd;break;case"drag":case"dragend":case"dragenter":case"dragexit":case"dragleave":case"dragover":case"dragstart":case"drop":Ge=uv;break;case"touchcancel":case"touchend":case"touchmove":case"touchstart":Ge=wv;break;case op:case ap:case lp:Ge=hv;break;case up:Ge=Av;break;case"scroll":Ge=av;break;case"wheel":Ge=Rv;break;case"copy":case"cut":case"paste":Ge=pv;break;case"gotpointercapture":case"lostpointercapture":case"pointercancel":case"pointerdown":case"pointermove":case"pointerout":case"pointerover":case"pointerup":Ge=Vd}var Ke=(r&4)!==0,jt=!Ke&&n==="scroll",J=Ke?Me!==null?Me+"Capture":null:Me;Ke=[];for(var j=ue,Q;j!==null;){Q=j;var be=Q.stateNode;if(Q.tag===5&&be!==null&&(Q=be,J!==null&&(be=$i(j,J),be!=null&&Ke.push(Ho(j,be,Q)))),jt)break;j=j.return}0<Ke.length&&(Me=new Ge(Me,qe,null,l,Ee),Ce.push({event:Me,listeners:Ke}))}}if((r&7)===0){e:{if(Me=n==="mouseover"||n==="pointerover",Ge=n==="mouseout"||n==="pointerout",Me&&l!==q&&(qe=l.relatedTarget||l.fromElement)&&(Or(qe)||qe[Li]))break e;if((Ge||Me)&&(Me=Ee.window===Ee?Ee:(Me=Ee.ownerDocument)?Me.defaultView||Me.parentWindow:window,Ge?(qe=l.relatedTarget||l.toElement,Ge=ue,qe=qe?Or(qe):null,qe!==null&&(jt=bi(qe),qe!==jt||qe.tag!==5&&qe.tag!==6)&&(qe=null)):(Ge=null,qe=ue),Ge!==qe)){if(Ke=kd,be="onMouseLeave",J="onMouseEnter",j="mouse",(n==="pointerout"||n==="pointerover")&&(Ke=Vd,be="onPointerLeave",J="onPointerEnter",j="pointer"),jt=Ge==null?Me:Ms(Ge),Q=qe==null?Me:Ms(qe),Me=new Ke(be,j+"leave",Ge,l,Ee),Me.target=jt,Me.relatedTarget=Q,be=null,Or(Ee)===ue&&(Ke=new Ke(J,j+"enter",qe,l,Ee),Ke.target=Q,Ke.relatedTarget=jt,be=Ke),jt=be,Ge&&qe)t:{for(Ke=Ge,J=qe,j=0,Q=Ke;Q;Q=xs(Q))j++;for(Q=0,be=J;be;be=xs(be))Q++;for(;0<j-Q;)Ke=xs(Ke),j--;for(;0<Q-j;)J=xs(J),Q--;for(;j--;){if(Ke===J||J!==null&&Ke===J.alternate)break t;Ke=xs(Ke),J=xs(J)}Ke=null}else Ke=null;Ge!==null&&mp(Ce,Me,Ge,Ke,!1),qe!==null&&jt!==null&&mp(Ce,jt,qe,Ke,!0)}}e:{if(Me=ue?Ms(ue):window,Ge=Me.nodeName&&Me.nodeName.toLowerCase(),Ge==="select"||Ge==="input"&&Me.type==="file")var $e=Uv;else if(Yd(Me))if($d)$e=zv;else{$e=Ov;var nt=Fv}else(Ge=Me.nodeName)&&Ge.toLowerCase()==="input"&&(Me.type==="checkbox"||Me.type==="radio")&&($e=Bv);if($e&&($e=$e(n,ue))){Kd(Ce,$e,l,Ee);break e}nt&&nt(n,Me,ue),n==="focusout"&&(nt=Me._wrapperState)&&nt.controlled&&Me.type==="number"&&We(Me,"number",Me.value)}switch(nt=ue?Ms(ue):window,n){case"focusin":(Yd(nt)||nt.contentEditable==="true")&&(vs=nt,Qu=ue,Bo=null);break;case"focusout":Bo=Qu=vs=null;break;case"mousedown":ec=!0;break;case"contextmenu":case"mouseup":case"dragend":ec=!1,rp(Ce,l,Ee);break;case"selectionchange":if(Vv)break;case"keydown":case"keyup":rp(Ce,l,Ee)}var it;if(Ku)e:{switch(n){case"compositionstart":var lt="onCompositionStart";break e;case"compositionend":lt="onCompositionEnd";break e;case"compositionupdate":lt="onCompositionUpdate";break e}lt=void 0}else _s?jd(n,l)&&(lt="onCompositionEnd"):n==="keydown"&&l.keyCode===229&&(lt="onCompositionStart");lt&&(Gd&&l.locale!=="ko"&&(_s||lt!=="onCompositionStart"?lt==="onCompositionEnd"&&_s&&(it=Bd()):(sr=Ee,Gu="value"in sr?sr.value:sr.textContent,_s=!0)),nt=ja(ue,lt),0<nt.length&&(lt=new Hd(lt,n,null,l,Ee),Ce.push({event:lt,listeners:nt}),it?lt.data=it:(it=qd(l),it!==null&&(lt.data=it)))),(it=bv?Lv(n,l):Dv(n,l))&&(ue=ja(ue,"onBeforeInput"),0<ue.length&&(Ee=new Hd("onBeforeInput","beforeinput",null,l,Ee),Ce.push({event:Ee,listeners:ue}),Ee.data=it))}dp(Ce,r)})}function Ho(n,r,l){return{instance:n,listener:r,currentTarget:l}}function ja(n,r){for(var l=r+"Capture",f=[];n!==null;){var p=n,v=p.stateNode;p.tag===5&&v!==null&&(p=v,v=$i(n,l),v!=null&&f.unshift(Ho(n,v,p)),v=$i(n,r),v!=null&&f.push(Ho(n,v,p))),n=n.return}return f}function xs(n){if(n===null)return null;do n=n.return;while(n&&n.tag!==5);return n||null}function mp(n,r,l,f,p){for(var v=r._reactName,A=[];l!==null&&l!==f;){var B=l,V=B.alternate,ue=B.stateNode;if(V!==null&&V===f)break;B.tag===5&&ue!==null&&(B=ue,p?(V=$i(l,v),V!=null&&A.unshift(Ho(l,V,B))):p||(V=$i(l,v),V!=null&&A.push(Ho(l,V,B)))),l=l.return}A.length!==0&&n.push({event:r,listeners:A})}var jv=/\r\n?/g,qv=/\u0000|\uFFFD/g;function gp(n){return(typeof n=="string"?n:""+n).replace(jv,`
`).replace(qv,"")}function qa(n,r,l){if(r=gp(r),gp(n)!==r&&l)throw Error(t(425))}function Ya(){}var oc=null,ac=null;function lc(n,r){return n==="textarea"||n==="noscript"||typeof r.children=="string"||typeof r.children=="number"||typeof r.dangerouslySetInnerHTML=="object"&&r.dangerouslySetInnerHTML!==null&&r.dangerouslySetInnerHTML.__html!=null}var uc=typeof setTimeout=="function"?setTimeout:void 0,Yv=typeof clearTimeout=="function"?clearTimeout:void 0,_p=typeof Promise=="function"?Promise:void 0,Kv=typeof queueMicrotask=="function"?queueMicrotask:typeof _p<"u"?function(n){return _p.resolve(null).then(n).catch($v)}:uc;function $v(n){setTimeout(function(){throw n})}function cc(n,r){var l=r,f=0;do{var p=l.nextSibling;if(n.removeChild(l),p&&p.nodeType===8)if(l=p.data,l==="/$"){if(f===0){n.removeChild(p),Lo(r);return}f--}else l!=="$"&&l!=="$?"&&l!=="$!"||f++;l=p}while(l);Lo(r)}function ar(n){for(;n!=null;n=n.nextSibling){var r=n.nodeType;if(r===1||r===3)break;if(r===8){if(r=n.data,r==="$"||r==="$!"||r==="$?")break;if(r==="/$")return null}}return n}function vp(n){n=n.previousSibling;for(var r=0;n;){if(n.nodeType===8){var l=n.data;if(l==="$"||l==="$!"||l==="$?"){if(r===0)return n;r--}else l==="/$"&&r++}n=n.previousSibling}return null}var Ss=Math.random().toString(36).slice(2),_i="__reactFiber$"+Ss,Vo="__reactProps$"+Ss,Li="__reactContainer$"+Ss,fc="__reactEvents$"+Ss,Zv="__reactListeners$"+Ss,Jv="__reactHandles$"+Ss;function Or(n){var r=n[_i];if(r)return r;for(var l=n.parentNode;l;){if(r=l[Li]||l[_i]){if(l=r.alternate,r.child!==null||l!==null&&l.child!==null)for(n=vp(n);n!==null;){if(l=n[_i])return l;n=vp(n)}return r}n=l,l=n.parentNode}return null}function Go(n){return n=n[_i]||n[Li],!n||n.tag!==5&&n.tag!==6&&n.tag!==13&&n.tag!==3?null:n}function Ms(n){if(n.tag===5||n.tag===6)return n.stateNode;throw Error(t(33))}function Ka(n){return n[Vo]||null}var hc=[],Es=-1;function lr(n){return{current:n}}function Ut(n){0>Es||(n.current=hc[Es],hc[Es]=null,Es--)}function Dt(n,r){Es++,hc[Es]=n.current,n.current=r}var ur={},_n=lr(ur),bn=lr(!1),Br=ur;function ws(n,r){var l=n.type.contextTypes;if(!l)return ur;var f=n.stateNode;if(f&&f.__reactInternalMemoizedUnmaskedChildContext===r)return f.__reactInternalMemoizedMaskedChildContext;var p={},v;for(v in l)p[v]=r[v];return f&&(n=n.stateNode,n.__reactInternalMemoizedUnmaskedChildContext=r,n.__reactInternalMemoizedMaskedChildContext=p),p}function Ln(n){return n=n.childContextTypes,n!=null}function $a(){Ut(bn),Ut(_n)}function yp(n,r,l){if(_n.current!==ur)throw Error(t(168));Dt(_n,r),Dt(bn,l)}function xp(n,r,l){var f=n.stateNode;if(r=r.childContextTypes,typeof f.getChildContext!="function")return l;f=f.getChildContext();for(var p in f)if(!(p in r))throw Error(t(108,de(n)||"Unknown",p));return re({},l,f)}function Za(n){return n=(n=n.stateNode)&&n.__reactInternalMemoizedMergedChildContext||ur,Br=_n.current,Dt(_n,n),Dt(bn,bn.current),!0}function Sp(n,r,l){var f=n.stateNode;if(!f)throw Error(t(169));l?(n=xp(n,r,Br),f.__reactInternalMemoizedMergedChildContext=n,Ut(bn),Ut(_n),Dt(_n,n)):Ut(bn),Dt(bn,l)}var Di=null,Ja=!1,dc=!1;function Mp(n){Di===null?Di=[n]:Di.push(n)}function Qv(n){Ja=!0,Mp(n)}function cr(){if(!dc&&Di!==null){dc=!0;var n=0,r=Ct;try{var l=Di;for(Ct=1;n<l.length;n++){var f=l[n];do f=f(!0);while(f!==null)}Di=null,Ja=!1}catch(p){throw Di!==null&&(Di=Di.slice(n+1)),$(st,cr),p}finally{Ct=r,dc=!1}}return null}var Ts=[],As=0,Qa=null,el=0,Kn=[],$n=0,zr=null,Ni=1,Ii="";function kr(n,r){Ts[As++]=el,Ts[As++]=Qa,Qa=n,el=r}function Ep(n,r,l){Kn[$n++]=Ni,Kn[$n++]=Ii,Kn[$n++]=zr,zr=n;var f=Ni;n=Ii;var p=32-At(f)-1;f&=~(1<<p),l+=1;var v=32-At(r)+p;if(30<v){var A=p-p%5;v=(f&(1<<A)-1).toString(32),f>>=A,p-=A,Ni=1<<32-At(r)+p|l<<p|f,Ii=v+n}else Ni=1<<v|l<<p|f,Ii=n}function pc(n){n.return!==null&&(kr(n,1),Ep(n,1,0))}function mc(n){for(;n===Qa;)Qa=Ts[--As],Ts[As]=null,el=Ts[--As],Ts[As]=null;for(;n===zr;)zr=Kn[--$n],Kn[$n]=null,Ii=Kn[--$n],Kn[$n]=null,Ni=Kn[--$n],Kn[$n]=null}var Wn=null,Xn=null,Bt=!1,ai=null;function wp(n,r){var l=ei(5,null,null,0);l.elementType="DELETED",l.stateNode=r,l.return=n,r=n.deletions,r===null?(n.deletions=[l],n.flags|=16):r.push(l)}function Tp(n,r){switch(n.tag){case 5:var l=n.type;return r=r.nodeType!==1||l.toLowerCase()!==r.nodeName.toLowerCase()?null:r,r!==null?(n.stateNode=r,Wn=n,Xn=ar(r.firstChild),!0):!1;case 6:return r=n.pendingProps===""||r.nodeType!==3?null:r,r!==null?(n.stateNode=r,Wn=n,Xn=null,!0):!1;case 13:return r=r.nodeType!==8?null:r,r!==null?(l=zr!==null?{id:Ni,overflow:Ii}:null,n.memoizedState={dehydrated:r,treeContext:l,retryLane:1073741824},l=ei(18,null,null,0),l.stateNode=r,l.return=n,n.child=l,Wn=n,Xn=null,!0):!1;default:return!1}}function gc(n){return(n.mode&1)!==0&&(n.flags&128)===0}function _c(n){if(Bt){var r=Xn;if(r){var l=r;if(!Tp(n,r)){if(gc(n))throw Error(t(418));r=ar(l.nextSibling);var f=Wn;r&&Tp(n,r)?wp(f,l):(n.flags=n.flags&-4097|2,Bt=!1,Wn=n)}}else{if(gc(n))throw Error(t(418));n.flags=n.flags&-4097|2,Bt=!1,Wn=n}}}function Ap(n){for(n=n.return;n!==null&&n.tag!==5&&n.tag!==3&&n.tag!==13;)n=n.return;Wn=n}function tl(n){if(n!==Wn)return!1;if(!Bt)return Ap(n),Bt=!0,!1;var r;if((r=n.tag!==3)&&!(r=n.tag!==5)&&(r=n.type,r=r!=="head"&&r!=="body"&&!lc(n.type,n.memoizedProps)),r&&(r=Xn)){if(gc(n))throw Cp(),Error(t(418));for(;r;)wp(n,r),r=ar(r.nextSibling)}if(Ap(n),n.tag===13){if(n=n.memoizedState,n=n!==null?n.dehydrated:null,!n)throw Error(t(317));e:{for(n=n.nextSibling,r=0;n;){if(n.nodeType===8){var l=n.data;if(l==="/$"){if(r===0){Xn=ar(n.nextSibling);break e}r--}else l!=="$"&&l!=="$!"&&l!=="$?"||r++}n=n.nextSibling}Xn=null}}else Xn=Wn?ar(n.stateNode.nextSibling):null;return!0}function Cp(){for(var n=Xn;n;)n=ar(n.nextSibling)}function Cs(){Xn=Wn=null,Bt=!1}function vc(n){ai===null?ai=[n]:ai.push(n)}var ey=P.ReactCurrentBatchConfig;function Wo(n,r,l){if(n=l.ref,n!==null&&typeof n!="function"&&typeof n!="object"){if(l._owner){if(l=l._owner,l){if(l.tag!==1)throw Error(t(309));var f=l.stateNode}if(!f)throw Error(t(147,n));var p=f,v=""+n;return r!==null&&r.ref!==null&&typeof r.ref=="function"&&r.ref._stringRef===v?r.ref:(r=function(A){var B=p.refs;A===null?delete B[v]:B[v]=A},r._stringRef=v,r)}if(typeof n!="string")throw Error(t(284));if(!l._owner)throw Error(t(290,n))}return n}function nl(n,r){throw n=Object.prototype.toString.call(r),Error(t(31,n==="[object Object]"?"object with keys {"+Object.keys(r).join(", ")+"}":n))}function Rp(n){var r=n._init;return r(n._payload)}function Pp(n){function r(J,j){if(n){var Q=J.deletions;Q===null?(J.deletions=[j],J.flags|=16):Q.push(j)}}function l(J,j){if(!n)return null;for(;j!==null;)r(J,j),j=j.sibling;return null}function f(J,j){for(J=new Map;j!==null;)j.key!==null?J.set(j.key,j):J.set(j.index,j),j=j.sibling;return J}function p(J,j){return J=vr(J,j),J.index=0,J.sibling=null,J}function v(J,j,Q){return J.index=Q,n?(Q=J.alternate,Q!==null?(Q=Q.index,Q<j?(J.flags|=2,j):Q):(J.flags|=2,j)):(J.flags|=1048576,j)}function A(J){return n&&J.alternate===null&&(J.flags|=2),J}function B(J,j,Q,be){return j===null||j.tag!==6?(j=cf(Q,J.mode,be),j.return=J,j):(j=p(j,Q),j.return=J,j)}function V(J,j,Q,be){var $e=Q.type;return $e===N?Ee(J,j,Q.props.children,be,Q.key):j!==null&&(j.elementType===$e||typeof $e=="object"&&$e!==null&&$e.$$typeof===te&&Rp($e)===j.type)?(be=p(j,Q.props),be.ref=Wo(J,j,Q),be.return=J,be):(be=Al(Q.type,Q.key,Q.props,null,J.mode,be),be.ref=Wo(J,j,Q),be.return=J,be)}function ue(J,j,Q,be){return j===null||j.tag!==4||j.stateNode.containerInfo!==Q.containerInfo||j.stateNode.implementation!==Q.implementation?(j=ff(Q,J.mode,be),j.return=J,j):(j=p(j,Q.children||[]),j.return=J,j)}function Ee(J,j,Q,be,$e){return j===null||j.tag!==7?(j=Yr(Q,J.mode,be,$e),j.return=J,j):(j=p(j,Q),j.return=J,j)}function Ce(J,j,Q){if(typeof j=="string"&&j!==""||typeof j=="number")return j=cf(""+j,J.mode,Q),j.return=J,j;if(typeof j=="object"&&j!==null){switch(j.$$typeof){case H:return Q=Al(j.type,j.key,j.props,null,J.mode,Q),Q.ref=Wo(J,null,j),Q.return=J,Q;case D:return j=ff(j,J.mode,Q),j.return=J,j;case te:var be=j._init;return Ce(J,be(j._payload),Q)}if(F(j)||he(j))return j=Yr(j,J.mode,Q,null),j.return=J,j;nl(J,j)}return null}function Me(J,j,Q,be){var $e=j!==null?j.key:null;if(typeof Q=="string"&&Q!==""||typeof Q=="number")return $e!==null?null:B(J,j,""+Q,be);if(typeof Q=="object"&&Q!==null){switch(Q.$$typeof){case H:return Q.key===$e?V(J,j,Q,be):null;case D:return Q.key===$e?ue(J,j,Q,be):null;case te:return $e=Q._init,Me(J,j,$e(Q._payload),be)}if(F(Q)||he(Q))return $e!==null?null:Ee(J,j,Q,be,null);nl(J,Q)}return null}function Ge(J,j,Q,be,$e){if(typeof be=="string"&&be!==""||typeof be=="number")return J=J.get(Q)||null,B(j,J,""+be,$e);if(typeof be=="object"&&be!==null){switch(be.$$typeof){case H:return J=J.get(be.key===null?Q:be.key)||null,V(j,J,be,$e);case D:return J=J.get(be.key===null?Q:be.key)||null,ue(j,J,be,$e);case te:var nt=be._init;return Ge(J,j,Q,nt(be._payload),$e)}if(F(be)||he(be))return J=J.get(Q)||null,Ee(j,J,be,$e,null);nl(j,be)}return null}function qe(J,j,Q,be){for(var $e=null,nt=null,it=j,lt=j=0,an=null;it!==null&&lt<Q.length;lt++){it.index>lt?(an=it,it=null):an=it.sibling;var wt=Me(J,it,Q[lt],be);if(wt===null){it===null&&(it=an);break}n&&it&&wt.alternate===null&&r(J,it),j=v(wt,j,lt),nt===null?$e=wt:nt.sibling=wt,nt=wt,it=an}if(lt===Q.length)return l(J,it),Bt&&kr(J,lt),$e;if(it===null){for(;lt<Q.length;lt++)it=Ce(J,Q[lt],be),it!==null&&(j=v(it,j,lt),nt===null?$e=it:nt.sibling=it,nt=it);return Bt&&kr(J,lt),$e}for(it=f(J,it);lt<Q.length;lt++)an=Ge(it,J,lt,Q[lt],be),an!==null&&(n&&an.alternate!==null&&it.delete(an.key===null?lt:an.key),j=v(an,j,lt),nt===null?$e=an:nt.sibling=an,nt=an);return n&&it.forEach(function(yr){return r(J,yr)}),Bt&&kr(J,lt),$e}function Ke(J,j,Q,be){var $e=he(Q);if(typeof $e!="function")throw Error(t(150));if(Q=$e.call(Q),Q==null)throw Error(t(151));for(var nt=$e=null,it=j,lt=j=0,an=null,wt=Q.next();it!==null&&!wt.done;lt++,wt=Q.next()){it.index>lt?(an=it,it=null):an=it.sibling;var yr=Me(J,it,wt.value,be);if(yr===null){it===null&&(it=an);break}n&&it&&yr.alternate===null&&r(J,it),j=v(yr,j,lt),nt===null?$e=yr:nt.sibling=yr,nt=yr,it=an}if(wt.done)return l(J,it),Bt&&kr(J,lt),$e;if(it===null){for(;!wt.done;lt++,wt=Q.next())wt=Ce(J,wt.value,be),wt!==null&&(j=v(wt,j,lt),nt===null?$e=wt:nt.sibling=wt,nt=wt);return Bt&&kr(J,lt),$e}for(it=f(J,it);!wt.done;lt++,wt=Q.next())wt=Ge(it,J,lt,wt.value,be),wt!==null&&(n&&wt.alternate!==null&&it.delete(wt.key===null?lt:wt.key),j=v(wt,j,lt),nt===null?$e=wt:nt.sibling=wt,nt=wt);return n&&it.forEach(function(Ny){return r(J,Ny)}),Bt&&kr(J,lt),$e}function jt(J,j,Q,be){if(typeof Q=="object"&&Q!==null&&Q.type===N&&Q.key===null&&(Q=Q.props.children),typeof Q=="object"&&Q!==null){switch(Q.$$typeof){case H:e:{for(var $e=Q.key,nt=j;nt!==null;){if(nt.key===$e){if($e=Q.type,$e===N){if(nt.tag===7){l(J,nt.sibling),j=p(nt,Q.props.children),j.return=J,J=j;break e}}else if(nt.elementType===$e||typeof $e=="object"&&$e!==null&&$e.$$typeof===te&&Rp($e)===nt.type){l(J,nt.sibling),j=p(nt,Q.props),j.ref=Wo(J,nt,Q),j.return=J,J=j;break e}l(J,nt);break}else r(J,nt);nt=nt.sibling}Q.type===N?(j=Yr(Q.props.children,J.mode,be,Q.key),j.return=J,J=j):(be=Al(Q.type,Q.key,Q.props,null,J.mode,be),be.ref=Wo(J,j,Q),be.return=J,J=be)}return A(J);case D:e:{for(nt=Q.key;j!==null;){if(j.key===nt)if(j.tag===4&&j.stateNode.containerInfo===Q.containerInfo&&j.stateNode.implementation===Q.implementation){l(J,j.sibling),j=p(j,Q.children||[]),j.return=J,J=j;break e}else{l(J,j);break}else r(J,j);j=j.sibling}j=ff(Q,J.mode,be),j.return=J,J=j}return A(J);case te:return nt=Q._init,jt(J,j,nt(Q._payload),be)}if(F(Q))return qe(J,j,Q,be);if(he(Q))return Ke(J,j,Q,be);nl(J,Q)}return typeof Q=="string"&&Q!==""||typeof Q=="number"?(Q=""+Q,j!==null&&j.tag===6?(l(J,j.sibling),j=p(j,Q),j.return=J,J=j):(l(J,j),j=cf(Q,J.mode,be),j.return=J,J=j),A(J)):l(J,j)}return jt}var Rs=Pp(!0),bp=Pp(!1),il=lr(null),rl=null,Ps=null,yc=null;function xc(){yc=Ps=rl=null}function Sc(n){var r=il.current;Ut(il),n._currentValue=r}function Mc(n,r,l){for(;n!==null;){var f=n.alternate;if((n.childLanes&r)!==r?(n.childLanes|=r,f!==null&&(f.childLanes|=r)):f!==null&&(f.childLanes&r)!==r&&(f.childLanes|=r),n===l)break;n=n.return}}function bs(n,r){rl=n,yc=Ps=null,n=n.dependencies,n!==null&&n.firstContext!==null&&((n.lanes&r)!==0&&(Dn=!0),n.firstContext=null)}function Zn(n){var r=n._currentValue;if(yc!==n)if(n={context:n,memoizedValue:r,next:null},Ps===null){if(rl===null)throw Error(t(308));Ps=n,rl.dependencies={lanes:0,firstContext:n}}else Ps=Ps.next=n;return r}var Hr=null;function Ec(n){Hr===null?Hr=[n]:Hr.push(n)}function Lp(n,r,l,f){var p=r.interleaved;return p===null?(l.next=l,Ec(r)):(l.next=p.next,p.next=l),r.interleaved=l,Ui(n,f)}function Ui(n,r){n.lanes|=r;var l=n.alternate;for(l!==null&&(l.lanes|=r),l=n,n=n.return;n!==null;)n.childLanes|=r,l=n.alternate,l!==null&&(l.childLanes|=r),l=n,n=n.return;return l.tag===3?l.stateNode:null}var fr=!1;function wc(n){n.updateQueue={baseState:n.memoizedState,firstBaseUpdate:null,lastBaseUpdate:null,shared:{pending:null,interleaved:null,lanes:0},effects:null}}function Dp(n,r){n=n.updateQueue,r.updateQueue===n&&(r.updateQueue={baseState:n.baseState,firstBaseUpdate:n.firstBaseUpdate,lastBaseUpdate:n.lastBaseUpdate,shared:n.shared,effects:n.effects})}function Fi(n,r){return{eventTime:n,lane:r,tag:0,payload:null,callback:null,next:null}}function hr(n,r,l){var f=n.updateQueue;if(f===null)return null;if(f=f.shared,(Mt&2)!==0){var p=f.pending;return p===null?r.next=r:(r.next=p.next,p.next=r),f.pending=r,Ui(n,l)}return p=f.interleaved,p===null?(r.next=r,Ec(f)):(r.next=p.next,p.next=r),f.interleaved=r,Ui(n,l)}function sl(n,r,l){if(r=r.updateQueue,r!==null&&(r=r.shared,(l&4194240)!==0)){var f=r.lanes;f&=n.pendingLanes,l|=f,r.lanes=l,Bu(n,l)}}function Np(n,r){var l=n.updateQueue,f=n.alternate;if(f!==null&&(f=f.updateQueue,l===f)){var p=null,v=null;if(l=l.firstBaseUpdate,l!==null){do{var A={eventTime:l.eventTime,lane:l.lane,tag:l.tag,payload:l.payload,callback:l.callback,next:null};v===null?p=v=A:v=v.next=A,l=l.next}while(l!==null);v===null?p=v=r:v=v.next=r}else p=v=r;l={baseState:f.baseState,firstBaseUpdate:p,lastBaseUpdate:v,shared:f.shared,effects:f.effects},n.updateQueue=l;return}n=l.lastBaseUpdate,n===null?l.firstBaseUpdate=r:n.next=r,l.lastBaseUpdate=r}function ol(n,r,l,f){var p=n.updateQueue;fr=!1;var v=p.firstBaseUpdate,A=p.lastBaseUpdate,B=p.shared.pending;if(B!==null){p.shared.pending=null;var V=B,ue=V.next;V.next=null,A===null?v=ue:A.next=ue,A=V;var Ee=n.alternate;Ee!==null&&(Ee=Ee.updateQueue,B=Ee.lastBaseUpdate,B!==A&&(B===null?Ee.firstBaseUpdate=ue:B.next=ue,Ee.lastBaseUpdate=V))}if(v!==null){var Ce=p.baseState;A=0,Ee=ue=V=null,B=v;do{var Me=B.lane,Ge=B.eventTime;if((f&Me)===Me){Ee!==null&&(Ee=Ee.next={eventTime:Ge,lane:0,tag:B.tag,payload:B.payload,callback:B.callback,next:null});e:{var qe=n,Ke=B;switch(Me=r,Ge=l,Ke.tag){case 1:if(qe=Ke.payload,typeof qe=="function"){Ce=qe.call(Ge,Ce,Me);break e}Ce=qe;break e;case 3:qe.flags=qe.flags&-65537|128;case 0:if(qe=Ke.payload,Me=typeof qe=="function"?qe.call(Ge,Ce,Me):qe,Me==null)break e;Ce=re({},Ce,Me);break e;case 2:fr=!0}}B.callback!==null&&B.lane!==0&&(n.flags|=64,Me=p.effects,Me===null?p.effects=[B]:Me.push(B))}else Ge={eventTime:Ge,lane:Me,tag:B.tag,payload:B.payload,callback:B.callback,next:null},Ee===null?(ue=Ee=Ge,V=Ce):Ee=Ee.next=Ge,A|=Me;if(B=B.next,B===null){if(B=p.shared.pending,B===null)break;Me=B,B=Me.next,Me.next=null,p.lastBaseUpdate=Me,p.shared.pending=null}}while(!0);if(Ee===null&&(V=Ce),p.baseState=V,p.firstBaseUpdate=ue,p.lastBaseUpdate=Ee,r=p.shared.interleaved,r!==null){p=r;do A|=p.lane,p=p.next;while(p!==r)}else v===null&&(p.shared.lanes=0);Wr|=A,n.lanes=A,n.memoizedState=Ce}}function Ip(n,r,l){if(n=r.effects,r.effects=null,n!==null)for(r=0;r<n.length;r++){var f=n[r],p=f.callback;if(p!==null){if(f.callback=null,f=l,typeof p!="function")throw Error(t(191,p));p.call(f)}}}var Xo={},vi=lr(Xo),jo=lr(Xo),qo=lr(Xo);function Vr(n){if(n===Xo)throw Error(t(174));return n}function Tc(n,r){switch(Dt(qo,r),Dt(jo,n),Dt(vi,Xo),n=r.nodeType,n){case 9:case 11:r=(r=r.documentElement)?r.namespaceURI:Fe(null,"");break;default:n=n===8?r.parentNode:r,r=n.namespaceURI||null,n=n.tagName,r=Fe(r,n)}Ut(vi),Dt(vi,r)}function Ls(){Ut(vi),Ut(jo),Ut(qo)}function Up(n){Vr(qo.current);var r=Vr(vi.current),l=Fe(r,n.type);r!==l&&(Dt(jo,n),Dt(vi,l))}function Ac(n){jo.current===n&&(Ut(vi),Ut(jo))}var Vt=lr(0);function al(n){for(var r=n;r!==null;){if(r.tag===13){var l=r.memoizedState;if(l!==null&&(l=l.dehydrated,l===null||l.data==="$?"||l.data==="$!"))return r}else if(r.tag===19&&r.memoizedProps.revealOrder!==void 0){if((r.flags&128)!==0)return r}else if(r.child!==null){r.child.return=r,r=r.child;continue}if(r===n)break;for(;r.sibling===null;){if(r.return===null||r.return===n)return null;r=r.return}r.sibling.return=r.return,r=r.sibling}return null}var Cc=[];function Rc(){for(var n=0;n<Cc.length;n++)Cc[n]._workInProgressVersionPrimary=null;Cc.length=0}var ll=P.ReactCurrentDispatcher,Pc=P.ReactCurrentBatchConfig,Gr=0,Gt=null,$t=null,sn=null,ul=!1,Yo=!1,Ko=0,ty=0;function vn(){throw Error(t(321))}function bc(n,r){if(r===null)return!1;for(var l=0;l<r.length&&l<n.length;l++)if(!oi(n[l],r[l]))return!1;return!0}function Lc(n,r,l,f,p,v){if(Gr=v,Gt=r,r.memoizedState=null,r.updateQueue=null,r.lanes=0,ll.current=n===null||n.memoizedState===null?sy:oy,n=l(f,p),Yo){v=0;do{if(Yo=!1,Ko=0,25<=v)throw Error(t(301));v+=1,sn=$t=null,r.updateQueue=null,ll.current=ay,n=l(f,p)}while(Yo)}if(ll.current=hl,r=$t!==null&&$t.next!==null,Gr=0,sn=$t=Gt=null,ul=!1,r)throw Error(t(300));return n}function Dc(){var n=Ko!==0;return Ko=0,n}function yi(){var n={memoizedState:null,baseState:null,baseQueue:null,queue:null,next:null};return sn===null?Gt.memoizedState=sn=n:sn=sn.next=n,sn}function Jn(){if($t===null){var n=Gt.alternate;n=n!==null?n.memoizedState:null}else n=$t.next;var r=sn===null?Gt.memoizedState:sn.next;if(r!==null)sn=r,$t=n;else{if(n===null)throw Error(t(310));$t=n,n={memoizedState:$t.memoizedState,baseState:$t.baseState,baseQueue:$t.baseQueue,queue:$t.queue,next:null},sn===null?Gt.memoizedState=sn=n:sn=sn.next=n}return sn}function $o(n,r){return typeof r=="function"?r(n):r}function Nc(n){var r=Jn(),l=r.queue;if(l===null)throw Error(t(311));l.lastRenderedReducer=n;var f=$t,p=f.baseQueue,v=l.pending;if(v!==null){if(p!==null){var A=p.next;p.next=v.next,v.next=A}f.baseQueue=p=v,l.pending=null}if(p!==null){v=p.next,f=f.baseState;var B=A=null,V=null,ue=v;do{var Ee=ue.lane;if((Gr&Ee)===Ee)V!==null&&(V=V.next={lane:0,action:ue.action,hasEagerState:ue.hasEagerState,eagerState:ue.eagerState,next:null}),f=ue.hasEagerState?ue.eagerState:n(f,ue.action);else{var Ce={lane:Ee,action:ue.action,hasEagerState:ue.hasEagerState,eagerState:ue.eagerState,next:null};V===null?(B=V=Ce,A=f):V=V.next=Ce,Gt.lanes|=Ee,Wr|=Ee}ue=ue.next}while(ue!==null&&ue!==v);V===null?A=f:V.next=B,oi(f,r.memoizedState)||(Dn=!0),r.memoizedState=f,r.baseState=A,r.baseQueue=V,l.lastRenderedState=f}if(n=l.interleaved,n!==null){p=n;do v=p.lane,Gt.lanes|=v,Wr|=v,p=p.next;while(p!==n)}else p===null&&(l.lanes=0);return[r.memoizedState,l.dispatch]}function Ic(n){var r=Jn(),l=r.queue;if(l===null)throw Error(t(311));l.lastRenderedReducer=n;var f=l.dispatch,p=l.pending,v=r.memoizedState;if(p!==null){l.pending=null;var A=p=p.next;do v=n(v,A.action),A=A.next;while(A!==p);oi(v,r.memoizedState)||(Dn=!0),r.memoizedState=v,r.baseQueue===null&&(r.baseState=v),l.lastRenderedState=v}return[v,f]}function Fp(){}function Op(n,r){var l=Gt,f=Jn(),p=r(),v=!oi(f.memoizedState,p);if(v&&(f.memoizedState=p,Dn=!0),f=f.queue,Uc(kp.bind(null,l,f,n),[n]),f.getSnapshot!==r||v||sn!==null&&sn.memoizedState.tag&1){if(l.flags|=2048,Zo(9,zp.bind(null,l,f,p,r),void 0,null),on===null)throw Error(t(349));(Gr&30)!==0||Bp(l,r,p)}return p}function Bp(n,r,l){n.flags|=16384,n={getSnapshot:r,value:l},r=Gt.updateQueue,r===null?(r={lastEffect:null,stores:null},Gt.updateQueue=r,r.stores=[n]):(l=r.stores,l===null?r.stores=[n]:l.push(n))}function zp(n,r,l,f){r.value=l,r.getSnapshot=f,Hp(r)&&Vp(n)}function kp(n,r,l){return l(function(){Hp(r)&&Vp(n)})}function Hp(n){var r=n.getSnapshot;n=n.value;try{var l=r();return!oi(n,l)}catch{return!0}}function Vp(n){var r=Ui(n,1);r!==null&&fi(r,n,1,-1)}function Gp(n){var r=yi();return typeof n=="function"&&(n=n()),r.memoizedState=r.baseState=n,n={pending:null,interleaved:null,lanes:0,dispatch:null,lastRenderedReducer:$o,lastRenderedState:n},r.queue=n,n=n.dispatch=ry.bind(null,Gt,n),[r.memoizedState,n]}function Zo(n,r,l,f){return n={tag:n,create:r,destroy:l,deps:f,next:null},r=Gt.updateQueue,r===null?(r={lastEffect:null,stores:null},Gt.updateQueue=r,r.lastEffect=n.next=n):(l=r.lastEffect,l===null?r.lastEffect=n.next=n:(f=l.next,l.next=n,n.next=f,r.lastEffect=n)),n}function Wp(){return Jn().memoizedState}function cl(n,r,l,f){var p=yi();Gt.flags|=n,p.memoizedState=Zo(1|r,l,void 0,f===void 0?null:f)}function fl(n,r,l,f){var p=Jn();f=f===void 0?null:f;var v=void 0;if($t!==null){var A=$t.memoizedState;if(v=A.destroy,f!==null&&bc(f,A.deps)){p.memoizedState=Zo(r,l,v,f);return}}Gt.flags|=n,p.memoizedState=Zo(1|r,l,v,f)}function Xp(n,r){return cl(8390656,8,n,r)}function Uc(n,r){return fl(2048,8,n,r)}function jp(n,r){return fl(4,2,n,r)}function qp(n,r){return fl(4,4,n,r)}function Yp(n,r){if(typeof r=="function")return n=n(),r(n),function(){r(null)};if(r!=null)return n=n(),r.current=n,function(){r.current=null}}function Kp(n,r,l){return l=l!=null?l.concat([n]):null,fl(4,4,Yp.bind(null,r,n),l)}function Fc(){}function $p(n,r){var l=Jn();r=r===void 0?null:r;var f=l.memoizedState;return f!==null&&r!==null&&bc(r,f[1])?f[0]:(l.memoizedState=[n,r],n)}function Zp(n,r){var l=Jn();r=r===void 0?null:r;var f=l.memoizedState;return f!==null&&r!==null&&bc(r,f[1])?f[0]:(n=n(),l.memoizedState=[n,r],n)}function Jp(n,r,l){return(Gr&21)===0?(n.baseState&&(n.baseState=!1,Dn=!0),n.memoizedState=l):(oi(l,r)||(l=Na(),Gt.lanes|=l,Wr|=l,n.baseState=!0),r)}function ny(n,r){var l=Ct;Ct=l!==0&&4>l?l:4,n(!0);var f=Pc.transition;Pc.transition={};try{n(!1),r()}finally{Ct=l,Pc.transition=f}}function Qp(){return Jn().memoizedState}function iy(n,r,l){var f=gr(n);if(l={lane:f,action:l,hasEagerState:!1,eagerState:null,next:null},em(n))tm(r,l);else if(l=Lp(n,r,l,f),l!==null){var p=wn();fi(l,n,f,p),nm(l,r,f)}}function ry(n,r,l){var f=gr(n),p={lane:f,action:l,hasEagerState:!1,eagerState:null,next:null};if(em(n))tm(r,p);else{var v=n.alternate;if(n.lanes===0&&(v===null||v.lanes===0)&&(v=r.lastRenderedReducer,v!==null))try{var A=r.lastRenderedState,B=v(A,l);if(p.hasEagerState=!0,p.eagerState=B,oi(B,A)){var V=r.interleaved;V===null?(p.next=p,Ec(r)):(p.next=V.next,V.next=p),r.interleaved=p;return}}catch{}finally{}l=Lp(n,r,p,f),l!==null&&(p=wn(),fi(l,n,f,p),nm(l,r,f))}}function em(n){var r=n.alternate;return n===Gt||r!==null&&r===Gt}function tm(n,r){Yo=ul=!0;var l=n.pending;l===null?r.next=r:(r.next=l.next,l.next=r),n.pending=r}function nm(n,r,l){if((l&4194240)!==0){var f=r.lanes;f&=n.pendingLanes,l|=f,r.lanes=l,Bu(n,l)}}var hl={readContext:Zn,useCallback:vn,useContext:vn,useEffect:vn,useImperativeHandle:vn,useInsertionEffect:vn,useLayoutEffect:vn,useMemo:vn,useReducer:vn,useRef:vn,useState:vn,useDebugValue:vn,useDeferredValue:vn,useTransition:vn,useMutableSource:vn,useSyncExternalStore:vn,useId:vn,unstable_isNewReconciler:!1},sy={readContext:Zn,useCallback:function(n,r){return yi().memoizedState=[n,r===void 0?null:r],n},useContext:Zn,useEffect:Xp,useImperativeHandle:function(n,r,l){return l=l!=null?l.concat([n]):null,cl(4194308,4,Yp.bind(null,r,n),l)},useLayoutEffect:function(n,r){return cl(4194308,4,n,r)},useInsertionEffect:function(n,r){return cl(4,2,n,r)},useMemo:function(n,r){var l=yi();return r=r===void 0?null:r,n=n(),l.memoizedState=[n,r],n},useReducer:function(n,r,l){var f=yi();return r=l!==void 0?l(r):r,f.memoizedState=f.baseState=r,n={pending:null,interleaved:null,lanes:0,dispatch:null,lastRenderedReducer:n,lastRenderedState:r},f.queue=n,n=n.dispatch=iy.bind(null,Gt,n),[f.memoizedState,n]},useRef:function(n){var r=yi();return n={current:n},r.memoizedState=n},useState:Gp,useDebugValue:Fc,useDeferredValue:function(n){return yi().memoizedState=n},useTransition:function(){var n=Gp(!1),r=n[0];return n=ny.bind(null,n[1]),yi().memoizedState=n,[r,n]},useMutableSource:function(){},useSyncExternalStore:function(n,r,l){var f=Gt,p=yi();if(Bt){if(l===void 0)throw Error(t(407));l=l()}else{if(l=r(),on===null)throw Error(t(349));(Gr&30)!==0||Bp(f,r,l)}p.memoizedState=l;var v={value:l,getSnapshot:r};return p.queue=v,Xp(kp.bind(null,f,v,n),[n]),f.flags|=2048,Zo(9,zp.bind(null,f,v,l,r),void 0,null),l},useId:function(){var n=yi(),r=on.identifierPrefix;if(Bt){var l=Ii,f=Ni;l=(f&~(1<<32-At(f)-1)).toString(32)+l,r=":"+r+"R"+l,l=Ko++,0<l&&(r+="H"+l.toString(32)),r+=":"}else l=ty++,r=":"+r+"r"+l.toString(32)+":";return n.memoizedState=r},unstable_isNewReconciler:!1},oy={readContext:Zn,useCallback:$p,useContext:Zn,useEffect:Uc,useImperativeHandle:Kp,useInsertionEffect:jp,useLayoutEffect:qp,useMemo:Zp,useReducer:Nc,useRef:Wp,useState:function(){return Nc($o)},useDebugValue:Fc,useDeferredValue:function(n){var r=Jn();return Jp(r,$t.memoizedState,n)},useTransition:function(){var n=Nc($o)[0],r=Jn().memoizedState;return[n,r]},useMutableSource:Fp,useSyncExternalStore:Op,useId:Qp,unstable_isNewReconciler:!1},ay={readContext:Zn,useCallback:$p,useContext:Zn,useEffect:Uc,useImperativeHandle:Kp,useInsertionEffect:jp,useLayoutEffect:qp,useMemo:Zp,useReducer:Ic,useRef:Wp,useState:function(){return Ic($o)},useDebugValue:Fc,useDeferredValue:function(n){var r=Jn();return $t===null?r.memoizedState=n:Jp(r,$t.memoizedState,n)},useTransition:function(){var n=Ic($o)[0],r=Jn().memoizedState;return[n,r]},useMutableSource:Fp,useSyncExternalStore:Op,useId:Qp,unstable_isNewReconciler:!1};function li(n,r){if(n&&n.defaultProps){r=re({},r),n=n.defaultProps;for(var l in n)r[l]===void 0&&(r[l]=n[l]);return r}return r}function Oc(n,r,l,f){r=n.memoizedState,l=l(f,r),l=l==null?r:re({},r,l),n.memoizedState=l,n.lanes===0&&(n.updateQueue.baseState=l)}var dl={isMounted:function(n){return(n=n._reactInternals)?bi(n)===n:!1},enqueueSetState:function(n,r,l){n=n._reactInternals;var f=wn(),p=gr(n),v=Fi(f,p);v.payload=r,l!=null&&(v.callback=l),r=hr(n,v,p),r!==null&&(fi(r,n,p,f),sl(r,n,p))},enqueueReplaceState:function(n,r,l){n=n._reactInternals;var f=wn(),p=gr(n),v=Fi(f,p);v.tag=1,v.payload=r,l!=null&&(v.callback=l),r=hr(n,v,p),r!==null&&(fi(r,n,p,f),sl(r,n,p))},enqueueForceUpdate:function(n,r){n=n._reactInternals;var l=wn(),f=gr(n),p=Fi(l,f);p.tag=2,r!=null&&(p.callback=r),r=hr(n,p,f),r!==null&&(fi(r,n,f,l),sl(r,n,f))}};function im(n,r,l,f,p,v,A){return n=n.stateNode,typeof n.shouldComponentUpdate=="function"?n.shouldComponentUpdate(f,v,A):r.prototype&&r.prototype.isPureReactComponent?!Oo(l,f)||!Oo(p,v):!0}function rm(n,r,l){var f=!1,p=ur,v=r.contextType;return typeof v=="object"&&v!==null?v=Zn(v):(p=Ln(r)?Br:_n.current,f=r.contextTypes,v=(f=f!=null)?ws(n,p):ur),r=new r(l,v),n.memoizedState=r.state!==null&&r.state!==void 0?r.state:null,r.updater=dl,n.stateNode=r,r._reactInternals=n,f&&(n=n.stateNode,n.__reactInternalMemoizedUnmaskedChildContext=p,n.__reactInternalMemoizedMaskedChildContext=v),r}function sm(n,r,l,f){n=r.state,typeof r.componentWillReceiveProps=="function"&&r.componentWillReceiveProps(l,f),typeof r.UNSAFE_componentWillReceiveProps=="function"&&r.UNSAFE_componentWillReceiveProps(l,f),r.state!==n&&dl.enqueueReplaceState(r,r.state,null)}function Bc(n,r,l,f){var p=n.stateNode;p.props=l,p.state=n.memoizedState,p.refs={},wc(n);var v=r.contextType;typeof v=="object"&&v!==null?p.context=Zn(v):(v=Ln(r)?Br:_n.current,p.context=ws(n,v)),p.state=n.memoizedState,v=r.getDerivedStateFromProps,typeof v=="function"&&(Oc(n,r,v,l),p.state=n.memoizedState),typeof r.getDerivedStateFromProps=="function"||typeof p.getSnapshotBeforeUpdate=="function"||typeof p.UNSAFE_componentWillMount!="function"&&typeof p.componentWillMount!="function"||(r=p.state,typeof p.componentWillMount=="function"&&p.componentWillMount(),typeof p.UNSAFE_componentWillMount=="function"&&p.UNSAFE_componentWillMount(),r!==p.state&&dl.enqueueReplaceState(p,p.state,null),ol(n,l,p,f),p.state=n.memoizedState),typeof p.componentDidMount=="function"&&(n.flags|=4194308)}function Ds(n,r){try{var l="",f=r;do l+=ne(f),f=f.return;while(f);var p=l}catch(v){p=`
Error generating stack: `+v.message+`
`+v.stack}return{value:n,source:r,stack:p,digest:null}}function zc(n,r,l){return{value:n,source:null,stack:l??null,digest:r??null}}function kc(n,r){try{console.error(r.value)}catch(l){setTimeout(function(){throw l})}}var ly=typeof WeakMap=="function"?WeakMap:Map;function om(n,r,l){l=Fi(-1,l),l.tag=3,l.payload={element:null};var f=r.value;return l.callback=function(){xl||(xl=!0,tf=f),kc(n,r)},l}function am(n,r,l){l=Fi(-1,l),l.tag=3;var f=n.type.getDerivedStateFromError;if(typeof f=="function"){var p=r.value;l.payload=function(){return f(p)},l.callback=function(){kc(n,r)}}var v=n.stateNode;return v!==null&&typeof v.componentDidCatch=="function"&&(l.callback=function(){kc(n,r),typeof f!="function"&&(pr===null?pr=new Set([this]):pr.add(this));var A=r.stack;this.componentDidCatch(r.value,{componentStack:A!==null?A:""})}),l}function lm(n,r,l){var f=n.pingCache;if(f===null){f=n.pingCache=new ly;var p=new Set;f.set(r,p)}else p=f.get(r),p===void 0&&(p=new Set,f.set(r,p));p.has(l)||(p.add(l),n=My.bind(null,n,r,l),r.then(n,n))}function um(n){do{var r;if((r=n.tag===13)&&(r=n.memoizedState,r=r!==null?r.dehydrated!==null:!0),r)return n;n=n.return}while(n!==null);return null}function cm(n,r,l,f,p){return(n.mode&1)===0?(n===r?n.flags|=65536:(n.flags|=128,l.flags|=131072,l.flags&=-52805,l.tag===1&&(l.alternate===null?l.tag=17:(r=Fi(-1,1),r.tag=2,hr(l,r,1))),l.lanes|=1),n):(n.flags|=65536,n.lanes=p,n)}var uy=P.ReactCurrentOwner,Dn=!1;function En(n,r,l,f){r.child=n===null?bp(r,null,l,f):Rs(r,n.child,l,f)}function fm(n,r,l,f,p){l=l.render;var v=r.ref;return bs(r,p),f=Lc(n,r,l,f,v,p),l=Dc(),n!==null&&!Dn?(r.updateQueue=n.updateQueue,r.flags&=-2053,n.lanes&=~p,Oi(n,r,p)):(Bt&&l&&pc(r),r.flags|=1,En(n,r,f,p),r.child)}function hm(n,r,l,f,p){if(n===null){var v=l.type;return typeof v=="function"&&!uf(v)&&v.defaultProps===void 0&&l.compare===null&&l.defaultProps===void 0?(r.tag=15,r.type=v,dm(n,r,v,f,p)):(n=Al(l.type,null,f,r,r.mode,p),n.ref=r.ref,n.return=r,r.child=n)}if(v=n.child,(n.lanes&p)===0){var A=v.memoizedProps;if(l=l.compare,l=l!==null?l:Oo,l(A,f)&&n.ref===r.ref)return Oi(n,r,p)}return r.flags|=1,n=vr(v,f),n.ref=r.ref,n.return=r,r.child=n}function dm(n,r,l,f,p){if(n!==null){var v=n.memoizedProps;if(Oo(v,f)&&n.ref===r.ref)if(Dn=!1,r.pendingProps=f=v,(n.lanes&p)!==0)(n.flags&131072)!==0&&(Dn=!0);else return r.lanes=n.lanes,Oi(n,r,p)}return Hc(n,r,l,f,p)}function pm(n,r,l){var f=r.pendingProps,p=f.children,v=n!==null?n.memoizedState:null;if(f.mode==="hidden")if((r.mode&1)===0)r.memoizedState={baseLanes:0,cachePool:null,transitions:null},Dt(Is,jn),jn|=l;else{if((l&1073741824)===0)return n=v!==null?v.baseLanes|l:l,r.lanes=r.childLanes=1073741824,r.memoizedState={baseLanes:n,cachePool:null,transitions:null},r.updateQueue=null,Dt(Is,jn),jn|=n,null;r.memoizedState={baseLanes:0,cachePool:null,transitions:null},f=v!==null?v.baseLanes:l,Dt(Is,jn),jn|=f}else v!==null?(f=v.baseLanes|l,r.memoizedState=null):f=l,Dt(Is,jn),jn|=f;return En(n,r,p,l),r.child}function mm(n,r){var l=r.ref;(n===null&&l!==null||n!==null&&n.ref!==l)&&(r.flags|=512,r.flags|=2097152)}function Hc(n,r,l,f,p){var v=Ln(l)?Br:_n.current;return v=ws(r,v),bs(r,p),l=Lc(n,r,l,f,v,p),f=Dc(),n!==null&&!Dn?(r.updateQueue=n.updateQueue,r.flags&=-2053,n.lanes&=~p,Oi(n,r,p)):(Bt&&f&&pc(r),r.flags|=1,En(n,r,l,p),r.child)}function gm(n,r,l,f,p){if(Ln(l)){var v=!0;Za(r)}else v=!1;if(bs(r,p),r.stateNode===null)ml(n,r),rm(r,l,f),Bc(r,l,f,p),f=!0;else if(n===null){var A=r.stateNode,B=r.memoizedProps;A.props=B;var V=A.context,ue=l.contextType;typeof ue=="object"&&ue!==null?ue=Zn(ue):(ue=Ln(l)?Br:_n.current,ue=ws(r,ue));var Ee=l.getDerivedStateFromProps,Ce=typeof Ee=="function"||typeof A.getSnapshotBeforeUpdate=="function";Ce||typeof A.UNSAFE_componentWillReceiveProps!="function"&&typeof A.componentWillReceiveProps!="function"||(B!==f||V!==ue)&&sm(r,A,f,ue),fr=!1;var Me=r.memoizedState;A.state=Me,ol(r,f,A,p),V=r.memoizedState,B!==f||Me!==V||bn.current||fr?(typeof Ee=="function"&&(Oc(r,l,Ee,f),V=r.memoizedState),(B=fr||im(r,l,B,f,Me,V,ue))?(Ce||typeof A.UNSAFE_componentWillMount!="function"&&typeof A.componentWillMount!="function"||(typeof A.componentWillMount=="function"&&A.componentWillMount(),typeof A.UNSAFE_componentWillMount=="function"&&A.UNSAFE_componentWillMount()),typeof A.componentDidMount=="function"&&(r.flags|=4194308)):(typeof A.componentDidMount=="function"&&(r.flags|=4194308),r.memoizedProps=f,r.memoizedState=V),A.props=f,A.state=V,A.context=ue,f=B):(typeof A.componentDidMount=="function"&&(r.flags|=4194308),f=!1)}else{A=r.stateNode,Dp(n,r),B=r.memoizedProps,ue=r.type===r.elementType?B:li(r.type,B),A.props=ue,Ce=r.pendingProps,Me=A.context,V=l.contextType,typeof V=="object"&&V!==null?V=Zn(V):(V=Ln(l)?Br:_n.current,V=ws(r,V));var Ge=l.getDerivedStateFromProps;(Ee=typeof Ge=="function"||typeof A.getSnapshotBeforeUpdate=="function")||typeof A.UNSAFE_componentWillReceiveProps!="function"&&typeof A.componentWillReceiveProps!="function"||(B!==Ce||Me!==V)&&sm(r,A,f,V),fr=!1,Me=r.memoizedState,A.state=Me,ol(r,f,A,p);var qe=r.memoizedState;B!==Ce||Me!==qe||bn.current||fr?(typeof Ge=="function"&&(Oc(r,l,Ge,f),qe=r.memoizedState),(ue=fr||im(r,l,ue,f,Me,qe,V)||!1)?(Ee||typeof A.UNSAFE_componentWillUpdate!="function"&&typeof A.componentWillUpdate!="function"||(typeof A.componentWillUpdate=="function"&&A.componentWillUpdate(f,qe,V),typeof A.UNSAFE_componentWillUpdate=="function"&&A.UNSAFE_componentWillUpdate(f,qe,V)),typeof A.componentDidUpdate=="function"&&(r.flags|=4),typeof A.getSnapshotBeforeUpdate=="function"&&(r.flags|=1024)):(typeof A.componentDidUpdate!="function"||B===n.memoizedProps&&Me===n.memoizedState||(r.flags|=4),typeof A.getSnapshotBeforeUpdate!="function"||B===n.memoizedProps&&Me===n.memoizedState||(r.flags|=1024),r.memoizedProps=f,r.memoizedState=qe),A.props=f,A.state=qe,A.context=V,f=ue):(typeof A.componentDidUpdate!="function"||B===n.memoizedProps&&Me===n.memoizedState||(r.flags|=4),typeof A.getSnapshotBeforeUpdate!="function"||B===n.memoizedProps&&Me===n.memoizedState||(r.flags|=1024),f=!1)}return Vc(n,r,l,f,v,p)}function Vc(n,r,l,f,p,v){mm(n,r);var A=(r.flags&128)!==0;if(!f&&!A)return p&&Sp(r,l,!1),Oi(n,r,v);f=r.stateNode,uy.current=r;var B=A&&typeof l.getDerivedStateFromError!="function"?null:f.render();return r.flags|=1,n!==null&&A?(r.child=Rs(r,n.child,null,v),r.child=Rs(r,null,B,v)):En(n,r,B,v),r.memoizedState=f.state,p&&Sp(r,l,!0),r.child}function _m(n){var r=n.stateNode;r.pendingContext?yp(n,r.pendingContext,r.pendingContext!==r.context):r.context&&yp(n,r.context,!1),Tc(n,r.containerInfo)}function vm(n,r,l,f,p){return Cs(),vc(p),r.flags|=256,En(n,r,l,f),r.child}var Gc={dehydrated:null,treeContext:null,retryLane:0};function Wc(n){return{baseLanes:n,cachePool:null,transitions:null}}function ym(n,r,l){var f=r.pendingProps,p=Vt.current,v=!1,A=(r.flags&128)!==0,B;if((B=A)||(B=n!==null&&n.memoizedState===null?!1:(p&2)!==0),B?(v=!0,r.flags&=-129):(n===null||n.memoizedState!==null)&&(p|=1),Dt(Vt,p&1),n===null)return _c(r),n=r.memoizedState,n!==null&&(n=n.dehydrated,n!==null)?((r.mode&1)===0?r.lanes=1:n.data==="$!"?r.lanes=8:r.lanes=1073741824,null):(A=f.children,n=f.fallback,v?(f=r.mode,v=r.child,A={mode:"hidden",children:A},(f&1)===0&&v!==null?(v.childLanes=0,v.pendingProps=A):v=Cl(A,f,0,null),n=Yr(n,f,l,null),v.return=r,n.return=r,v.sibling=n,r.child=v,r.child.memoizedState=Wc(l),r.memoizedState=Gc,n):Xc(r,A));if(p=n.memoizedState,p!==null&&(B=p.dehydrated,B!==null))return cy(n,r,A,f,B,p,l);if(v){v=f.fallback,A=r.mode,p=n.child,B=p.sibling;var V={mode:"hidden",children:f.children};return(A&1)===0&&r.child!==p?(f=r.child,f.childLanes=0,f.pendingProps=V,r.deletions=null):(f=vr(p,V),f.subtreeFlags=p.subtreeFlags&14680064),B!==null?v=vr(B,v):(v=Yr(v,A,l,null),v.flags|=2),v.return=r,f.return=r,f.sibling=v,r.child=f,f=v,v=r.child,A=n.child.memoizedState,A=A===null?Wc(l):{baseLanes:A.baseLanes|l,cachePool:null,transitions:A.transitions},v.memoizedState=A,v.childLanes=n.childLanes&~l,r.memoizedState=Gc,f}return v=n.child,n=v.sibling,f=vr(v,{mode:"visible",children:f.children}),(r.mode&1)===0&&(f.lanes=l),f.return=r,f.sibling=null,n!==null&&(l=r.deletions,l===null?(r.deletions=[n],r.flags|=16):l.push(n)),r.child=f,r.memoizedState=null,f}function Xc(n,r){return r=Cl({mode:"visible",children:r},n.mode,0,null),r.return=n,n.child=r}function pl(n,r,l,f){return f!==null&&vc(f),Rs(r,n.child,null,l),n=Xc(r,r.pendingProps.children),n.flags|=2,r.memoizedState=null,n}function cy(n,r,l,f,p,v,A){if(l)return r.flags&256?(r.flags&=-257,f=zc(Error(t(422))),pl(n,r,A,f)):r.memoizedState!==null?(r.child=n.child,r.flags|=128,null):(v=f.fallback,p=r.mode,f=Cl({mode:"visible",children:f.children},p,0,null),v=Yr(v,p,A,null),v.flags|=2,f.return=r,v.return=r,f.sibling=v,r.child=f,(r.mode&1)!==0&&Rs(r,n.child,null,A),r.child.memoizedState=Wc(A),r.memoizedState=Gc,v);if((r.mode&1)===0)return pl(n,r,A,null);if(p.data==="$!"){if(f=p.nextSibling&&p.nextSibling.dataset,f)var B=f.dgst;return f=B,v=Error(t(419)),f=zc(v,f,void 0),pl(n,r,A,f)}if(B=(A&n.childLanes)!==0,Dn||B){if(f=on,f!==null){switch(A&-A){case 4:p=2;break;case 16:p=8;break;case 64:case 128:case 256:case 512:case 1024:case 2048:case 4096:case 8192:case 16384:case 32768:case 65536:case 131072:case 262144:case 524288:case 1048576:case 2097152:case 4194304:case 8388608:case 16777216:case 33554432:case 67108864:p=32;break;case 536870912:p=268435456;break;default:p=0}p=(p&(f.suspendedLanes|A))!==0?0:p,p!==0&&p!==v.retryLane&&(v.retryLane=p,Ui(n,p),fi(f,n,p,-1))}return lf(),f=zc(Error(t(421))),pl(n,r,A,f)}return p.data==="$?"?(r.flags|=128,r.child=n.child,r=Ey.bind(null,n),p._reactRetry=r,null):(n=v.treeContext,Xn=ar(p.nextSibling),Wn=r,Bt=!0,ai=null,n!==null&&(Kn[$n++]=Ni,Kn[$n++]=Ii,Kn[$n++]=zr,Ni=n.id,Ii=n.overflow,zr=r),r=Xc(r,f.children),r.flags|=4096,r)}function xm(n,r,l){n.lanes|=r;var f=n.alternate;f!==null&&(f.lanes|=r),Mc(n.return,r,l)}function jc(n,r,l,f,p){var v=n.memoizedState;v===null?n.memoizedState={isBackwards:r,rendering:null,renderingStartTime:0,last:f,tail:l,tailMode:p}:(v.isBackwards=r,v.rendering=null,v.renderingStartTime=0,v.last=f,v.tail=l,v.tailMode=p)}function Sm(n,r,l){var f=r.pendingProps,p=f.revealOrder,v=f.tail;if(En(n,r,f.children,l),f=Vt.current,(f&2)!==0)f=f&1|2,r.flags|=128;else{if(n!==null&&(n.flags&128)!==0)e:for(n=r.child;n!==null;){if(n.tag===13)n.memoizedState!==null&&xm(n,l,r);else if(n.tag===19)xm(n,l,r);else if(n.child!==null){n.child.return=n,n=n.child;continue}if(n===r)break e;for(;n.sibling===null;){if(n.return===null||n.return===r)break e;n=n.return}n.sibling.return=n.return,n=n.sibling}f&=1}if(Dt(Vt,f),(r.mode&1)===0)r.memoizedState=null;else switch(p){case"forwards":for(l=r.child,p=null;l!==null;)n=l.alternate,n!==null&&al(n)===null&&(p=l),l=l.sibling;l=p,l===null?(p=r.child,r.child=null):(p=l.sibling,l.sibling=null),jc(r,!1,p,l,v);break;case"backwards":for(l=null,p=r.child,r.child=null;p!==null;){if(n=p.alternate,n!==null&&al(n)===null){r.child=p;break}n=p.sibling,p.sibling=l,l=p,p=n}jc(r,!0,l,null,v);break;case"together":jc(r,!1,null,null,void 0);break;default:r.memoizedState=null}return r.child}function ml(n,r){(r.mode&1)===0&&n!==null&&(n.alternate=null,r.alternate=null,r.flags|=2)}function Oi(n,r,l){if(n!==null&&(r.dependencies=n.dependencies),Wr|=r.lanes,(l&r.childLanes)===0)return null;if(n!==null&&r.child!==n.child)throw Error(t(153));if(r.child!==null){for(n=r.child,l=vr(n,n.pendingProps),r.child=l,l.return=r;n.sibling!==null;)n=n.sibling,l=l.sibling=vr(n,n.pendingProps),l.return=r;l.sibling=null}return r.child}function fy(n,r,l){switch(r.tag){case 3:_m(r),Cs();break;case 5:Up(r);break;case 1:Ln(r.type)&&Za(r);break;case 4:Tc(r,r.stateNode.containerInfo);break;case 10:var f=r.type._context,p=r.memoizedProps.value;Dt(il,f._currentValue),f._currentValue=p;break;case 13:if(f=r.memoizedState,f!==null)return f.dehydrated!==null?(Dt(Vt,Vt.current&1),r.flags|=128,null):(l&r.child.childLanes)!==0?ym(n,r,l):(Dt(Vt,Vt.current&1),n=Oi(n,r,l),n!==null?n.sibling:null);Dt(Vt,Vt.current&1);break;case 19:if(f=(l&r.childLanes)!==0,(n.flags&128)!==0){if(f)return Sm(n,r,l);r.flags|=128}if(p=r.memoizedState,p!==null&&(p.rendering=null,p.tail=null,p.lastEffect=null),Dt(Vt,Vt.current),f)break;return null;case 22:case 23:return r.lanes=0,pm(n,r,l)}return Oi(n,r,l)}var Mm,qc,Em,wm;Mm=function(n,r){for(var l=r.child;l!==null;){if(l.tag===5||l.tag===6)n.appendChild(l.stateNode);else if(l.tag!==4&&l.child!==null){l.child.return=l,l=l.child;continue}if(l===r)break;for(;l.sibling===null;){if(l.return===null||l.return===r)return;l=l.return}l.sibling.return=l.return,l=l.sibling}},qc=function(){},Em=function(n,r,l,f){var p=n.memoizedProps;if(p!==f){n=r.stateNode,Vr(vi.current);var v=null;switch(l){case"input":p=Se(n,p),f=Se(n,f),v=[];break;case"select":p=re({},p,{value:void 0}),f=re({},f,{value:void 0}),v=[];break;case"textarea":p=oe(n,p),f=oe(n,f),v=[];break;default:typeof p.onClick!="function"&&typeof f.onClick=="function"&&(n.onclick=Ya)}ft(l,f);var A;l=null;for(ue in p)if(!f.hasOwnProperty(ue)&&p.hasOwnProperty(ue)&&p[ue]!=null)if(ue==="style"){var B=p[ue];for(A in B)B.hasOwnProperty(A)&&(l||(l={}),l[A]="")}else ue!=="dangerouslySetInnerHTML"&&ue!=="children"&&ue!=="suppressContentEditableWarning"&&ue!=="suppressHydrationWarning"&&ue!=="autoFocus"&&(o.hasOwnProperty(ue)?v||(v=[]):(v=v||[]).push(ue,null));for(ue in f){var V=f[ue];if(B=p!=null?p[ue]:void 0,f.hasOwnProperty(ue)&&V!==B&&(V!=null||B!=null))if(ue==="style")if(B){for(A in B)!B.hasOwnProperty(A)||V&&V.hasOwnProperty(A)||(l||(l={}),l[A]="");for(A in V)V.hasOwnProperty(A)&&B[A]!==V[A]&&(l||(l={}),l[A]=V[A])}else l||(v||(v=[]),v.push(ue,l)),l=V;else ue==="dangerouslySetInnerHTML"?(V=V?V.__html:void 0,B=B?B.__html:void 0,V!=null&&B!==V&&(v=v||[]).push(ue,V)):ue==="children"?typeof V!="string"&&typeof V!="number"||(v=v||[]).push(ue,""+V):ue!=="suppressContentEditableWarning"&&ue!=="suppressHydrationWarning"&&(o.hasOwnProperty(ue)?(V!=null&&ue==="onScroll"&&It("scroll",n),v||B===V||(v=[])):(v=v||[]).push(ue,V))}l&&(v=v||[]).push("style",l);var ue=v;(r.updateQueue=ue)&&(r.flags|=4)}},wm=function(n,r,l,f){l!==f&&(r.flags|=4)};function Jo(n,r){if(!Bt)switch(n.tailMode){case"hidden":r=n.tail;for(var l=null;r!==null;)r.alternate!==null&&(l=r),r=r.sibling;l===null?n.tail=null:l.sibling=null;break;case"collapsed":l=n.tail;for(var f=null;l!==null;)l.alternate!==null&&(f=l),l=l.sibling;f===null?r||n.tail===null?n.tail=null:n.tail.sibling=null:f.sibling=null}}function yn(n){var r=n.alternate!==null&&n.alternate.child===n.child,l=0,f=0;if(r)for(var p=n.child;p!==null;)l|=p.lanes|p.childLanes,f|=p.subtreeFlags&14680064,f|=p.flags&14680064,p.return=n,p=p.sibling;else for(p=n.child;p!==null;)l|=p.lanes|p.childLanes,f|=p.subtreeFlags,f|=p.flags,p.return=n,p=p.sibling;return n.subtreeFlags|=f,n.childLanes=l,r}function hy(n,r,l){var f=r.pendingProps;switch(mc(r),r.tag){case 2:case 16:case 15:case 0:case 11:case 7:case 8:case 12:case 9:case 14:return yn(r),null;case 1:return Ln(r.type)&&$a(),yn(r),null;case 3:return f=r.stateNode,Ls(),Ut(bn),Ut(_n),Rc(),f.pendingContext&&(f.context=f.pendingContext,f.pendingContext=null),(n===null||n.child===null)&&(tl(r)?r.flags|=4:n===null||n.memoizedState.isDehydrated&&(r.flags&256)===0||(r.flags|=1024,ai!==null&&(sf(ai),ai=null))),qc(n,r),yn(r),null;case 5:Ac(r);var p=Vr(qo.current);if(l=r.type,n!==null&&r.stateNode!=null)Em(n,r,l,f,p),n.ref!==r.ref&&(r.flags|=512,r.flags|=2097152);else{if(!f){if(r.stateNode===null)throw Error(t(166));return yn(r),null}if(n=Vr(vi.current),tl(r)){f=r.stateNode,l=r.type;var v=r.memoizedProps;switch(f[_i]=r,f[Vo]=v,n=(r.mode&1)!==0,l){case"dialog":It("cancel",f),It("close",f);break;case"iframe":case"object":case"embed":It("load",f);break;case"video":case"audio":for(p=0;p<zo.length;p++)It(zo[p],f);break;case"source":It("error",f);break;case"img":case"image":case"link":It("error",f),It("load",f);break;case"details":It("toggle",f);break;case"input":Oe(f,v),It("invalid",f);break;case"select":f._wrapperState={wasMultiple:!!v.multiple},It("invalid",f);break;case"textarea":ve(f,v),It("invalid",f)}ft(l,v),p=null;for(var A in v)if(v.hasOwnProperty(A)){var B=v[A];A==="children"?typeof B=="string"?f.textContent!==B&&(v.suppressHydrationWarning!==!0&&qa(f.textContent,B,n),p=["children",B]):typeof B=="number"&&f.textContent!==""+B&&(v.suppressHydrationWarning!==!0&&qa(f.textContent,B,n),p=["children",""+B]):o.hasOwnProperty(A)&&B!=null&&A==="onScroll"&&It("scroll",f)}switch(l){case"input":tt(f),Be(f,v,!0);break;case"textarea":tt(f),ye(f);break;case"select":case"option":break;default:typeof v.onClick=="function"&&(f.onclick=Ya)}f=p,r.updateQueue=f,f!==null&&(r.flags|=4)}else{A=p.nodeType===9?p:p.ownerDocument,n==="http://www.w3.org/1999/xhtml"&&(n=Qe(l)),n==="http://www.w3.org/1999/xhtml"?l==="script"?(n=A.createElement("div"),n.innerHTML="<script><\/script>",n=n.removeChild(n.firstChild)):typeof f.is=="string"?n=A.createElement(l,{is:f.is}):(n=A.createElement(l),l==="select"&&(A=n,f.multiple?A.multiple=!0:f.size&&(A.size=f.size))):n=A.createElementNS(n,l),n[_i]=r,n[Vo]=f,Mm(n,r,!1,!1),r.stateNode=n;e:{switch(A=bt(l,f),l){case"dialog":It("cancel",n),It("close",n),p=f;break;case"iframe":case"object":case"embed":It("load",n),p=f;break;case"video":case"audio":for(p=0;p<zo.length;p++)It(zo[p],n);p=f;break;case"source":It("error",n),p=f;break;case"img":case"image":case"link":It("error",n),It("load",n),p=f;break;case"details":It("toggle",n),p=f;break;case"input":Oe(n,f),p=Se(n,f),It("invalid",n);break;case"option":p=f;break;case"select":n._wrapperState={wasMultiple:!!f.multiple},p=re({},f,{value:void 0}),It("invalid",n);break;case"textarea":ve(n,f),p=oe(n,f),It("invalid",n);break;default:p=f}ft(l,p),B=p;for(v in B)if(B.hasOwnProperty(v)){var V=B[v];v==="style"?Xe(n,V):v==="dangerouslySetInnerHTML"?(V=V?V.__html:void 0,V!=null&&ut(n,V)):v==="children"?typeof V=="string"?(l!=="textarea"||V!=="")&&Re(n,V):typeof V=="number"&&Re(n,""+V):v!=="suppressContentEditableWarning"&&v!=="suppressHydrationWarning"&&v!=="autoFocus"&&(o.hasOwnProperty(v)?V!=null&&v==="onScroll"&&It("scroll",n):V!=null&&T(n,v,V,A))}switch(l){case"input":tt(n),Be(n,f,!1);break;case"textarea":tt(n),ye(n);break;case"option":f.value!=null&&n.setAttribute("value",""+ee(f.value));break;case"select":n.multiple=!!f.multiple,v=f.value,v!=null?R(n,!!f.multiple,v,!1):f.defaultValue!=null&&R(n,!!f.multiple,f.defaultValue,!0);break;default:typeof p.onClick=="function"&&(n.onclick=Ya)}switch(l){case"button":case"input":case"select":case"textarea":f=!!f.autoFocus;break e;case"img":f=!0;break e;default:f=!1}}f&&(r.flags|=4)}r.ref!==null&&(r.flags|=512,r.flags|=2097152)}return yn(r),null;case 6:if(n&&r.stateNode!=null)wm(n,r,n.memoizedProps,f);else{if(typeof f!="string"&&r.stateNode===null)throw Error(t(166));if(l=Vr(qo.current),Vr(vi.current),tl(r)){if(f=r.stateNode,l=r.memoizedProps,f[_i]=r,(v=f.nodeValue!==l)&&(n=Wn,n!==null))switch(n.tag){case 3:qa(f.nodeValue,l,(n.mode&1)!==0);break;case 5:n.memoizedProps.suppressHydrationWarning!==!0&&qa(f.nodeValue,l,(n.mode&1)!==0)}v&&(r.flags|=4)}else f=(l.nodeType===9?l:l.ownerDocument).createTextNode(f),f[_i]=r,r.stateNode=f}return yn(r),null;case 13:if(Ut(Vt),f=r.memoizedState,n===null||n.memoizedState!==null&&n.memoizedState.dehydrated!==null){if(Bt&&Xn!==null&&(r.mode&1)!==0&&(r.flags&128)===0)Cp(),Cs(),r.flags|=98560,v=!1;else if(v=tl(r),f!==null&&f.dehydrated!==null){if(n===null){if(!v)throw Error(t(318));if(v=r.memoizedState,v=v!==null?v.dehydrated:null,!v)throw Error(t(317));v[_i]=r}else Cs(),(r.flags&128)===0&&(r.memoizedState=null),r.flags|=4;yn(r),v=!1}else ai!==null&&(sf(ai),ai=null),v=!0;if(!v)return r.flags&65536?r:null}return(r.flags&128)!==0?(r.lanes=l,r):(f=f!==null,f!==(n!==null&&n.memoizedState!==null)&&f&&(r.child.flags|=8192,(r.mode&1)!==0&&(n===null||(Vt.current&1)!==0?Zt===0&&(Zt=3):lf())),r.updateQueue!==null&&(r.flags|=4),yn(r),null);case 4:return Ls(),qc(n,r),n===null&&ko(r.stateNode.containerInfo),yn(r),null;case 10:return Sc(r.type._context),yn(r),null;case 17:return Ln(r.type)&&$a(),yn(r),null;case 19:if(Ut(Vt),v=r.memoizedState,v===null)return yn(r),null;if(f=(r.flags&128)!==0,A=v.rendering,A===null)if(f)Jo(v,!1);else{if(Zt!==0||n!==null&&(n.flags&128)!==0)for(n=r.child;n!==null;){if(A=al(n),A!==null){for(r.flags|=128,Jo(v,!1),f=A.updateQueue,f!==null&&(r.updateQueue=f,r.flags|=4),r.subtreeFlags=0,f=l,l=r.child;l!==null;)v=l,n=f,v.flags&=14680066,A=v.alternate,A===null?(v.childLanes=0,v.lanes=n,v.child=null,v.subtreeFlags=0,v.memoizedProps=null,v.memoizedState=null,v.updateQueue=null,v.dependencies=null,v.stateNode=null):(v.childLanes=A.childLanes,v.lanes=A.lanes,v.child=A.child,v.subtreeFlags=0,v.deletions=null,v.memoizedProps=A.memoizedProps,v.memoizedState=A.memoizedState,v.updateQueue=A.updateQueue,v.type=A.type,n=A.dependencies,v.dependencies=n===null?null:{lanes:n.lanes,firstContext:n.firstContext}),l=l.sibling;return Dt(Vt,Vt.current&1|2),r.child}n=n.sibling}v.tail!==null&&Ie()>Us&&(r.flags|=128,f=!0,Jo(v,!1),r.lanes=4194304)}else{if(!f)if(n=al(A),n!==null){if(r.flags|=128,f=!0,l=n.updateQueue,l!==null&&(r.updateQueue=l,r.flags|=4),Jo(v,!0),v.tail===null&&v.tailMode==="hidden"&&!A.alternate&&!Bt)return yn(r),null}else 2*Ie()-v.renderingStartTime>Us&&l!==1073741824&&(r.flags|=128,f=!0,Jo(v,!1),r.lanes=4194304);v.isBackwards?(A.sibling=r.child,r.child=A):(l=v.last,l!==null?l.sibling=A:r.child=A,v.last=A)}return v.tail!==null?(r=v.tail,v.rendering=r,v.tail=r.sibling,v.renderingStartTime=Ie(),r.sibling=null,l=Vt.current,Dt(Vt,f?l&1|2:l&1),r):(yn(r),null);case 22:case 23:return af(),f=r.memoizedState!==null,n!==null&&n.memoizedState!==null!==f&&(r.flags|=8192),f&&(r.mode&1)!==0?(jn&1073741824)!==0&&(yn(r),r.subtreeFlags&6&&(r.flags|=8192)):yn(r),null;case 24:return null;case 25:return null}throw Error(t(156,r.tag))}function dy(n,r){switch(mc(r),r.tag){case 1:return Ln(r.type)&&$a(),n=r.flags,n&65536?(r.flags=n&-65537|128,r):null;case 3:return Ls(),Ut(bn),Ut(_n),Rc(),n=r.flags,(n&65536)!==0&&(n&128)===0?(r.flags=n&-65537|128,r):null;case 5:return Ac(r),null;case 13:if(Ut(Vt),n=r.memoizedState,n!==null&&n.dehydrated!==null){if(r.alternate===null)throw Error(t(340));Cs()}return n=r.flags,n&65536?(r.flags=n&-65537|128,r):null;case 19:return Ut(Vt),null;case 4:return Ls(),null;case 10:return Sc(r.type._context),null;case 22:case 23:return af(),null;case 24:return null;default:return null}}var gl=!1,xn=!1,py=typeof WeakSet=="function"?WeakSet:Set,je=null;function Ns(n,r){var l=n.ref;if(l!==null)if(typeof l=="function")try{l(null)}catch(f){Xt(n,r,f)}else l.current=null}function Yc(n,r,l){try{l()}catch(f){Xt(n,r,f)}}var Tm=!1;function my(n,r){if(oc=Fa,n=ip(),Ju(n)){if("selectionStart"in n)var l={start:n.selectionStart,end:n.selectionEnd};else e:{l=(l=n.ownerDocument)&&l.defaultView||window;var f=l.getSelection&&l.getSelection();if(f&&f.rangeCount!==0){l=f.anchorNode;var p=f.anchorOffset,v=f.focusNode;f=f.focusOffset;try{l.nodeType,v.nodeType}catch{l=null;break e}var A=0,B=-1,V=-1,ue=0,Ee=0,Ce=n,Me=null;t:for(;;){for(var Ge;Ce!==l||p!==0&&Ce.nodeType!==3||(B=A+p),Ce!==v||f!==0&&Ce.nodeType!==3||(V=A+f),Ce.nodeType===3&&(A+=Ce.nodeValue.length),(Ge=Ce.firstChild)!==null;)Me=Ce,Ce=Ge;for(;;){if(Ce===n)break t;if(Me===l&&++ue===p&&(B=A),Me===v&&++Ee===f&&(V=A),(Ge=Ce.nextSibling)!==null)break;Ce=Me,Me=Ce.parentNode}Ce=Ge}l=B===-1||V===-1?null:{start:B,end:V}}else l=null}l=l||{start:0,end:0}}else l=null;for(ac={focusedElem:n,selectionRange:l},Fa=!1,je=r;je!==null;)if(r=je,n=r.child,(r.subtreeFlags&1028)!==0&&n!==null)n.return=r,je=n;else for(;je!==null;){r=je;try{var qe=r.alternate;if((r.flags&1024)!==0)switch(r.tag){case 0:case 11:case 15:break;case 1:if(qe!==null){var Ke=qe.memoizedProps,jt=qe.memoizedState,J=r.stateNode,j=J.getSnapshotBeforeUpdate(r.elementType===r.type?Ke:li(r.type,Ke),jt);J.__reactInternalSnapshotBeforeUpdate=j}break;case 3:var Q=r.stateNode.containerInfo;Q.nodeType===1?Q.textContent="":Q.nodeType===9&&Q.documentElement&&Q.removeChild(Q.documentElement);break;case 5:case 6:case 4:case 17:break;default:throw Error(t(163))}}catch(be){Xt(r,r.return,be)}if(n=r.sibling,n!==null){n.return=r.return,je=n;break}je=r.return}return qe=Tm,Tm=!1,qe}function Qo(n,r,l){var f=r.updateQueue;if(f=f!==null?f.lastEffect:null,f!==null){var p=f=f.next;do{if((p.tag&n)===n){var v=p.destroy;p.destroy=void 0,v!==void 0&&Yc(r,l,v)}p=p.next}while(p!==f)}}function _l(n,r){if(r=r.updateQueue,r=r!==null?r.lastEffect:null,r!==null){var l=r=r.next;do{if((l.tag&n)===n){var f=l.create;l.destroy=f()}l=l.next}while(l!==r)}}function Kc(n){var r=n.ref;if(r!==null){var l=n.stateNode;switch(n.tag){case 5:n=l;break;default:n=l}typeof r=="function"?r(n):r.current=n}}function Am(n){var r=n.alternate;r!==null&&(n.alternate=null,Am(r)),n.child=null,n.deletions=null,n.sibling=null,n.tag===5&&(r=n.stateNode,r!==null&&(delete r[_i],delete r[Vo],delete r[fc],delete r[Zv],delete r[Jv])),n.stateNode=null,n.return=null,n.dependencies=null,n.memoizedProps=null,n.memoizedState=null,n.pendingProps=null,n.stateNode=null,n.updateQueue=null}function Cm(n){return n.tag===5||n.tag===3||n.tag===4}function Rm(n){e:for(;;){for(;n.sibling===null;){if(n.return===null||Cm(n.return))return null;n=n.return}for(n.sibling.return=n.return,n=n.sibling;n.tag!==5&&n.tag!==6&&n.tag!==18;){if(n.flags&2||n.child===null||n.tag===4)continue e;n.child.return=n,n=n.child}if(!(n.flags&2))return n.stateNode}}function $c(n,r,l){var f=n.tag;if(f===5||f===6)n=n.stateNode,r?l.nodeType===8?l.parentNode.insertBefore(n,r):l.insertBefore(n,r):(l.nodeType===8?(r=l.parentNode,r.insertBefore(n,l)):(r=l,r.appendChild(n)),l=l._reactRootContainer,l!=null||r.onclick!==null||(r.onclick=Ya));else if(f!==4&&(n=n.child,n!==null))for($c(n,r,l),n=n.sibling;n!==null;)$c(n,r,l),n=n.sibling}function Zc(n,r,l){var f=n.tag;if(f===5||f===6)n=n.stateNode,r?l.insertBefore(n,r):l.appendChild(n);else if(f!==4&&(n=n.child,n!==null))for(Zc(n,r,l),n=n.sibling;n!==null;)Zc(n,r,l),n=n.sibling}var hn=null,ui=!1;function dr(n,r,l){for(l=l.child;l!==null;)Pm(n,r,l),l=l.sibling}function Pm(n,r,l){if(ht&&typeof ht.onCommitFiberUnmount=="function")try{ht.onCommitFiberUnmount(nn,l)}catch{}switch(l.tag){case 5:xn||Ns(l,r);case 6:var f=hn,p=ui;hn=null,dr(n,r,l),hn=f,ui=p,hn!==null&&(ui?(n=hn,l=l.stateNode,n.nodeType===8?n.parentNode.removeChild(l):n.removeChild(l)):hn.removeChild(l.stateNode));break;case 18:hn!==null&&(ui?(n=hn,l=l.stateNode,n.nodeType===8?cc(n.parentNode,l):n.nodeType===1&&cc(n,l),Lo(n)):cc(hn,l.stateNode));break;case 4:f=hn,p=ui,hn=l.stateNode.containerInfo,ui=!0,dr(n,r,l),hn=f,ui=p;break;case 0:case 11:case 14:case 15:if(!xn&&(f=l.updateQueue,f!==null&&(f=f.lastEffect,f!==null))){p=f=f.next;do{var v=p,A=v.destroy;v=v.tag,A!==void 0&&((v&2)!==0||(v&4)!==0)&&Yc(l,r,A),p=p.next}while(p!==f)}dr(n,r,l);break;case 1:if(!xn&&(Ns(l,r),f=l.stateNode,typeof f.componentWillUnmount=="function"))try{f.props=l.memoizedProps,f.state=l.memoizedState,f.componentWillUnmount()}catch(B){Xt(l,r,B)}dr(n,r,l);break;case 21:dr(n,r,l);break;case 22:l.mode&1?(xn=(f=xn)||l.memoizedState!==null,dr(n,r,l),xn=f):dr(n,r,l);break;default:dr(n,r,l)}}function bm(n){var r=n.updateQueue;if(r!==null){n.updateQueue=null;var l=n.stateNode;l===null&&(l=n.stateNode=new py),r.forEach(function(f){var p=wy.bind(null,n,f);l.has(f)||(l.add(f),f.then(p,p))})}}function ci(n,r){var l=r.deletions;if(l!==null)for(var f=0;f<l.length;f++){var p=l[f];try{var v=n,A=r,B=A;e:for(;B!==null;){switch(B.tag){case 5:hn=B.stateNode,ui=!1;break e;case 3:hn=B.stateNode.containerInfo,ui=!0;break e;case 4:hn=B.stateNode.containerInfo,ui=!0;break e}B=B.return}if(hn===null)throw Error(t(160));Pm(v,A,p),hn=null,ui=!1;var V=p.alternate;V!==null&&(V.return=null),p.return=null}catch(ue){Xt(p,r,ue)}}if(r.subtreeFlags&12854)for(r=r.child;r!==null;)Lm(r,n),r=r.sibling}function Lm(n,r){var l=n.alternate,f=n.flags;switch(n.tag){case 0:case 11:case 14:case 15:if(ci(r,n),xi(n),f&4){try{Qo(3,n,n.return),_l(3,n)}catch(Ke){Xt(n,n.return,Ke)}try{Qo(5,n,n.return)}catch(Ke){Xt(n,n.return,Ke)}}break;case 1:ci(r,n),xi(n),f&512&&l!==null&&Ns(l,l.return);break;case 5:if(ci(r,n),xi(n),f&512&&l!==null&&Ns(l,l.return),n.flags&32){var p=n.stateNode;try{Re(p,"")}catch(Ke){Xt(n,n.return,Ke)}}if(f&4&&(p=n.stateNode,p!=null)){var v=n.memoizedProps,A=l!==null?l.memoizedProps:v,B=n.type,V=n.updateQueue;if(n.updateQueue=null,V!==null)try{B==="input"&&v.type==="radio"&&v.name!=null&&pe(p,v),bt(B,A);var ue=bt(B,v);for(A=0;A<V.length;A+=2){var Ee=V[A],Ce=V[A+1];Ee==="style"?Xe(p,Ce):Ee==="dangerouslySetInnerHTML"?ut(p,Ce):Ee==="children"?Re(p,Ce):T(p,Ee,Ce,ue)}switch(B){case"input":et(p,v);break;case"textarea":_e(p,v);break;case"select":var Me=p._wrapperState.wasMultiple;p._wrapperState.wasMultiple=!!v.multiple;var Ge=v.value;Ge!=null?R(p,!!v.multiple,Ge,!1):Me!==!!v.multiple&&(v.defaultValue!=null?R(p,!!v.multiple,v.defaultValue,!0):R(p,!!v.multiple,v.multiple?[]:"",!1))}p[Vo]=v}catch(Ke){Xt(n,n.return,Ke)}}break;case 6:if(ci(r,n),xi(n),f&4){if(n.stateNode===null)throw Error(t(162));p=n.stateNode,v=n.memoizedProps;try{p.nodeValue=v}catch(Ke){Xt(n,n.return,Ke)}}break;case 3:if(ci(r,n),xi(n),f&4&&l!==null&&l.memoizedState.isDehydrated)try{Lo(r.containerInfo)}catch(Ke){Xt(n,n.return,Ke)}break;case 4:ci(r,n),xi(n);break;case 13:ci(r,n),xi(n),p=n.child,p.flags&8192&&(v=p.memoizedState!==null,p.stateNode.isHidden=v,!v||p.alternate!==null&&p.alternate.memoizedState!==null||(ef=Ie())),f&4&&bm(n);break;case 22:if(Ee=l!==null&&l.memoizedState!==null,n.mode&1?(xn=(ue=xn)||Ee,ci(r,n),xn=ue):ci(r,n),xi(n),f&8192){if(ue=n.memoizedState!==null,(n.stateNode.isHidden=ue)&&!Ee&&(n.mode&1)!==0)for(je=n,Ee=n.child;Ee!==null;){for(Ce=je=Ee;je!==null;){switch(Me=je,Ge=Me.child,Me.tag){case 0:case 11:case 14:case 15:Qo(4,Me,Me.return);break;case 1:Ns(Me,Me.return);var qe=Me.stateNode;if(typeof qe.componentWillUnmount=="function"){f=Me,l=Me.return;try{r=f,qe.props=r.memoizedProps,qe.state=r.memoizedState,qe.componentWillUnmount()}catch(Ke){Xt(f,l,Ke)}}break;case 5:Ns(Me,Me.return);break;case 22:if(Me.memoizedState!==null){Im(Ce);continue}}Ge!==null?(Ge.return=Me,je=Ge):Im(Ce)}Ee=Ee.sibling}e:for(Ee=null,Ce=n;;){if(Ce.tag===5){if(Ee===null){Ee=Ce;try{p=Ce.stateNode,ue?(v=p.style,typeof v.setProperty=="function"?v.setProperty("display","none","important"):v.display="none"):(B=Ce.stateNode,V=Ce.memoizedProps.style,A=V!=null&&V.hasOwnProperty("display")?V.display:null,B.style.display=at("display",A))}catch(Ke){Xt(n,n.return,Ke)}}}else if(Ce.tag===6){if(Ee===null)try{Ce.stateNode.nodeValue=ue?"":Ce.memoizedProps}catch(Ke){Xt(n,n.return,Ke)}}else if((Ce.tag!==22&&Ce.tag!==23||Ce.memoizedState===null||Ce===n)&&Ce.child!==null){Ce.child.return=Ce,Ce=Ce.child;continue}if(Ce===n)break e;for(;Ce.sibling===null;){if(Ce.return===null||Ce.return===n)break e;Ee===Ce&&(Ee=null),Ce=Ce.return}Ee===Ce&&(Ee=null),Ce.sibling.return=Ce.return,Ce=Ce.sibling}}break;case 19:ci(r,n),xi(n),f&4&&bm(n);break;case 21:break;default:ci(r,n),xi(n)}}function xi(n){var r=n.flags;if(r&2){try{e:{for(var l=n.return;l!==null;){if(Cm(l)){var f=l;break e}l=l.return}throw Error(t(160))}switch(f.tag){case 5:var p=f.stateNode;f.flags&32&&(Re(p,""),f.flags&=-33);var v=Rm(n);Zc(n,v,p);break;case 3:case 4:var A=f.stateNode.containerInfo,B=Rm(n);$c(n,B,A);break;default:throw Error(t(161))}}catch(V){Xt(n,n.return,V)}n.flags&=-3}r&4096&&(n.flags&=-4097)}function gy(n,r,l){je=n,Dm(n)}function Dm(n,r,l){for(var f=(n.mode&1)!==0;je!==null;){var p=je,v=p.child;if(p.tag===22&&f){var A=p.memoizedState!==null||gl;if(!A){var B=p.alternate,V=B!==null&&B.memoizedState!==null||xn;B=gl;var ue=xn;if(gl=A,(xn=V)&&!ue)for(je=p;je!==null;)A=je,V=A.child,A.tag===22&&A.memoizedState!==null?Um(p):V!==null?(V.return=A,je=V):Um(p);for(;v!==null;)je=v,Dm(v),v=v.sibling;je=p,gl=B,xn=ue}Nm(n)}else(p.subtreeFlags&8772)!==0&&v!==null?(v.return=p,je=v):Nm(n)}}function Nm(n){for(;je!==null;){var r=je;if((r.flags&8772)!==0){var l=r.alternate;try{if((r.flags&8772)!==0)switch(r.tag){case 0:case 11:case 15:xn||_l(5,r);break;case 1:var f=r.stateNode;if(r.flags&4&&!xn)if(l===null)f.componentDidMount();else{var p=r.elementType===r.type?l.memoizedProps:li(r.type,l.memoizedProps);f.componentDidUpdate(p,l.memoizedState,f.__reactInternalSnapshotBeforeUpdate)}var v=r.updateQueue;v!==null&&Ip(r,v,f);break;case 3:var A=r.updateQueue;if(A!==null){if(l=null,r.child!==null)switch(r.child.tag){case 5:l=r.child.stateNode;break;case 1:l=r.child.stateNode}Ip(r,A,l)}break;case 5:var B=r.stateNode;if(l===null&&r.flags&4){l=B;var V=r.memoizedProps;switch(r.type){case"button":case"input":case"select":case"textarea":V.autoFocus&&l.focus();break;case"img":V.src&&(l.src=V.src)}}break;case 6:break;case 4:break;case 12:break;case 13:if(r.memoizedState===null){var ue=r.alternate;if(ue!==null){var Ee=ue.memoizedState;if(Ee!==null){var Ce=Ee.dehydrated;Ce!==null&&Lo(Ce)}}}break;case 19:case 17:case 21:case 22:case 23:case 25:break;default:throw Error(t(163))}xn||r.flags&512&&Kc(r)}catch(Me){Xt(r,r.return,Me)}}if(r===n){je=null;break}if(l=r.sibling,l!==null){l.return=r.return,je=l;break}je=r.return}}function Im(n){for(;je!==null;){var r=je;if(r===n){je=null;break}var l=r.sibling;if(l!==null){l.return=r.return,je=l;break}je=r.return}}function Um(n){for(;je!==null;){var r=je;try{switch(r.tag){case 0:case 11:case 15:var l=r.return;try{_l(4,r)}catch(V){Xt(r,l,V)}break;case 1:var f=r.stateNode;if(typeof f.componentDidMount=="function"){var p=r.return;try{f.componentDidMount()}catch(V){Xt(r,p,V)}}var v=r.return;try{Kc(r)}catch(V){Xt(r,v,V)}break;case 5:var A=r.return;try{Kc(r)}catch(V){Xt(r,A,V)}}}catch(V){Xt(r,r.return,V)}if(r===n){je=null;break}var B=r.sibling;if(B!==null){B.return=r.return,je=B;break}je=r.return}}var _y=Math.ceil,vl=P.ReactCurrentDispatcher,Jc=P.ReactCurrentOwner,Qn=P.ReactCurrentBatchConfig,Mt=0,on=null,Yt=null,dn=0,jn=0,Is=lr(0),Zt=0,ea=null,Wr=0,yl=0,Qc=0,ta=null,Nn=null,ef=0,Us=1/0,Bi=null,xl=!1,tf=null,pr=null,Sl=!1,mr=null,Ml=0,na=0,nf=null,El=-1,wl=0;function wn(){return(Mt&6)!==0?Ie():El!==-1?El:El=Ie()}function gr(n){return(n.mode&1)===0?1:(Mt&2)!==0&&dn!==0?dn&-dn:ey.transition!==null?(wl===0&&(wl=Na()),wl):(n=Ct,n!==0||(n=window.event,n=n===void 0?16:Od(n.type)),n)}function fi(n,r,l,f){if(50<na)throw na=0,nf=null,Error(t(185));Ao(n,l,f),((Mt&2)===0||n!==on)&&(n===on&&((Mt&2)===0&&(yl|=l),Zt===4&&_r(n,dn)),In(n,f),l===1&&Mt===0&&(r.mode&1)===0&&(Us=Ie()+500,Ja&&cr()))}function In(n,r){var l=n.callbackNode;Pn(n,r);var f=Vn(n,n===on?dn:0);if(f===0)l!==null&&De(l),n.callbackNode=null,n.callbackPriority=0;else if(r=f&-f,n.callbackPriority!==r){if(l!=null&&De(l),r===1)n.tag===0?Qv(Om.bind(null,n)):Mp(Om.bind(null,n)),Kv(function(){(Mt&6)===0&&cr()}),l=null;else{switch(Pd(f)){case 1:l=st;break;case 4:l=Ze;break;case 16:l=xt;break;case 536870912:l=Nt;break;default:l=xt}l=Xm(l,Fm.bind(null,n))}n.callbackPriority=r,n.callbackNode=l}}function Fm(n,r){if(El=-1,wl=0,(Mt&6)!==0)throw Error(t(327));var l=n.callbackNode;if(Fs()&&n.callbackNode!==l)return null;var f=Vn(n,n===on?dn:0);if(f===0)return null;if((f&30)!==0||(f&n.expiredLanes)!==0||r)r=Tl(n,f);else{r=f;var p=Mt;Mt|=2;var v=zm();(on!==n||dn!==r)&&(Bi=null,Us=Ie()+500,jr(n,r));do try{xy();break}catch(B){Bm(n,B)}while(!0);xc(),vl.current=v,Mt=p,Yt!==null?r=0:(on=null,dn=0,r=Zt)}if(r!==0){if(r===2&&(p=Fr(n),p!==0&&(f=p,r=rf(n,p))),r===1)throw l=ea,jr(n,0),_r(n,f),In(n,Ie()),l;if(r===6)_r(n,f);else{if(p=n.current.alternate,(f&30)===0&&!vy(p)&&(r=Tl(n,f),r===2&&(v=Fr(n),v!==0&&(f=v,r=rf(n,v))),r===1))throw l=ea,jr(n,0),_r(n,f),In(n,Ie()),l;switch(n.finishedWork=p,n.finishedLanes=f,r){case 0:case 1:throw Error(t(345));case 2:qr(n,Nn,Bi);break;case 3:if(_r(n,f),(f&130023424)===f&&(r=ef+500-Ie(),10<r)){if(Vn(n,0)!==0)break;if(p=n.suspendedLanes,(p&f)!==f){wn(),n.pingedLanes|=n.suspendedLanes&p;break}n.timeoutHandle=uc(qr.bind(null,n,Nn,Bi),r);break}qr(n,Nn,Bi);break;case 4:if(_r(n,f),(f&4194240)===f)break;for(r=n.eventTimes,p=-1;0<f;){var A=31-At(f);v=1<<A,A=r[A],A>p&&(p=A),f&=~v}if(f=p,f=Ie()-f,f=(120>f?120:480>f?480:1080>f?1080:1920>f?1920:3e3>f?3e3:4320>f?4320:1960*_y(f/1960))-f,10<f){n.timeoutHandle=uc(qr.bind(null,n,Nn,Bi),f);break}qr(n,Nn,Bi);break;case 5:qr(n,Nn,Bi);break;default:throw Error(t(329))}}}return In(n,Ie()),n.callbackNode===l?Fm.bind(null,n):null}function rf(n,r){var l=ta;return n.current.memoizedState.isDehydrated&&(jr(n,r).flags|=256),n=Tl(n,r),n!==2&&(r=Nn,Nn=l,r!==null&&sf(r)),n}function sf(n){Nn===null?Nn=n:Nn.push.apply(Nn,n)}function vy(n){for(var r=n;;){if(r.flags&16384){var l=r.updateQueue;if(l!==null&&(l=l.stores,l!==null))for(var f=0;f<l.length;f++){var p=l[f],v=p.getSnapshot;p=p.value;try{if(!oi(v(),p))return!1}catch{return!1}}}if(l=r.child,r.subtreeFlags&16384&&l!==null)l.return=r,r=l;else{if(r===n)break;for(;r.sibling===null;){if(r.return===null||r.return===n)return!0;r=r.return}r.sibling.return=r.return,r=r.sibling}}return!0}function _r(n,r){for(r&=~Qc,r&=~yl,n.suspendedLanes|=r,n.pingedLanes&=~r,n=n.expirationTimes;0<r;){var l=31-At(r),f=1<<l;n[l]=-1,r&=~f}}function Om(n){if((Mt&6)!==0)throw Error(t(327));Fs();var r=Vn(n,0);if((r&1)===0)return In(n,Ie()),null;var l=Tl(n,r);if(n.tag!==0&&l===2){var f=Fr(n);f!==0&&(r=f,l=rf(n,f))}if(l===1)throw l=ea,jr(n,0),_r(n,r),In(n,Ie()),l;if(l===6)throw Error(t(345));return n.finishedWork=n.current.alternate,n.finishedLanes=r,qr(n,Nn,Bi),In(n,Ie()),null}function of(n,r){var l=Mt;Mt|=1;try{return n(r)}finally{Mt=l,Mt===0&&(Us=Ie()+500,Ja&&cr())}}function Xr(n){mr!==null&&mr.tag===0&&(Mt&6)===0&&Fs();var r=Mt;Mt|=1;var l=Qn.transition,f=Ct;try{if(Qn.transition=null,Ct=1,n)return n()}finally{Ct=f,Qn.transition=l,Mt=r,(Mt&6)===0&&cr()}}function af(){jn=Is.current,Ut(Is)}function jr(n,r){n.finishedWork=null,n.finishedLanes=0;var l=n.timeoutHandle;if(l!==-1&&(n.timeoutHandle=-1,Yv(l)),Yt!==null)for(l=Yt.return;l!==null;){var f=l;switch(mc(f),f.tag){case 1:f=f.type.childContextTypes,f!=null&&$a();break;case 3:Ls(),Ut(bn),Ut(_n),Rc();break;case 5:Ac(f);break;case 4:Ls();break;case 13:Ut(Vt);break;case 19:Ut(Vt);break;case 10:Sc(f.type._context);break;case 22:case 23:af()}l=l.return}if(on=n,Yt=n=vr(n.current,null),dn=jn=r,Zt=0,ea=null,Qc=yl=Wr=0,Nn=ta=null,Hr!==null){for(r=0;r<Hr.length;r++)if(l=Hr[r],f=l.interleaved,f!==null){l.interleaved=null;var p=f.next,v=l.pending;if(v!==null){var A=v.next;v.next=p,f.next=A}l.pending=f}Hr=null}return n}function Bm(n,r){do{var l=Yt;try{if(xc(),ll.current=hl,ul){for(var f=Gt.memoizedState;f!==null;){var p=f.queue;p!==null&&(p.pending=null),f=f.next}ul=!1}if(Gr=0,sn=$t=Gt=null,Yo=!1,Ko=0,Jc.current=null,l===null||l.return===null){Zt=1,ea=r,Yt=null;break}e:{var v=n,A=l.return,B=l,V=r;if(r=dn,B.flags|=32768,V!==null&&typeof V=="object"&&typeof V.then=="function"){var ue=V,Ee=B,Ce=Ee.tag;if((Ee.mode&1)===0&&(Ce===0||Ce===11||Ce===15)){var Me=Ee.alternate;Me?(Ee.updateQueue=Me.updateQueue,Ee.memoizedState=Me.memoizedState,Ee.lanes=Me.lanes):(Ee.updateQueue=null,Ee.memoizedState=null)}var Ge=um(A);if(Ge!==null){Ge.flags&=-257,cm(Ge,A,B,v,r),Ge.mode&1&&lm(v,ue,r),r=Ge,V=ue;var qe=r.updateQueue;if(qe===null){var Ke=new Set;Ke.add(V),r.updateQueue=Ke}else qe.add(V);break e}else{if((r&1)===0){lm(v,ue,r),lf();break e}V=Error(t(426))}}else if(Bt&&B.mode&1){var jt=um(A);if(jt!==null){(jt.flags&65536)===0&&(jt.flags|=256),cm(jt,A,B,v,r),vc(Ds(V,B));break e}}v=V=Ds(V,B),Zt!==4&&(Zt=2),ta===null?ta=[v]:ta.push(v),v=A;do{switch(v.tag){case 3:v.flags|=65536,r&=-r,v.lanes|=r;var J=om(v,V,r);Np(v,J);break e;case 1:B=V;var j=v.type,Q=v.stateNode;if((v.flags&128)===0&&(typeof j.getDerivedStateFromError=="function"||Q!==null&&typeof Q.componentDidCatch=="function"&&(pr===null||!pr.has(Q)))){v.flags|=65536,r&=-r,v.lanes|=r;var be=am(v,B,r);Np(v,be);break e}}v=v.return}while(v!==null)}Hm(l)}catch($e){r=$e,Yt===l&&l!==null&&(Yt=l=l.return);continue}break}while(!0)}function zm(){var n=vl.current;return vl.current=hl,n===null?hl:n}function lf(){(Zt===0||Zt===3||Zt===2)&&(Zt=4),on===null||(Wr&268435455)===0&&(yl&268435455)===0||_r(on,dn)}function Tl(n,r){var l=Mt;Mt|=2;var f=zm();(on!==n||dn!==r)&&(Bi=null,jr(n,r));do try{yy();break}catch(p){Bm(n,p)}while(!0);if(xc(),Mt=l,vl.current=f,Yt!==null)throw Error(t(261));return on=null,dn=0,Zt}function yy(){for(;Yt!==null;)km(Yt)}function xy(){for(;Yt!==null&&!Ve();)km(Yt)}function km(n){var r=Wm(n.alternate,n,jn);n.memoizedProps=n.pendingProps,r===null?Hm(n):Yt=r,Jc.current=null}function Hm(n){var r=n;do{var l=r.alternate;if(n=r.return,(r.flags&32768)===0){if(l=hy(l,r,jn),l!==null){Yt=l;return}}else{if(l=dy(l,r),l!==null){l.flags&=32767,Yt=l;return}if(n!==null)n.flags|=32768,n.subtreeFlags=0,n.deletions=null;else{Zt=6,Yt=null;return}}if(r=r.sibling,r!==null){Yt=r;return}Yt=r=n}while(r!==null);Zt===0&&(Zt=5)}function qr(n,r,l){var f=Ct,p=Qn.transition;try{Qn.transition=null,Ct=1,Sy(n,r,l,f)}finally{Qn.transition=p,Ct=f}return null}function Sy(n,r,l,f){do Fs();while(mr!==null);if((Mt&6)!==0)throw Error(t(327));l=n.finishedWork;var p=n.finishedLanes;if(l===null)return null;if(n.finishedWork=null,n.finishedLanes=0,l===n.current)throw Error(t(177));n.callbackNode=null,n.callbackPriority=0;var v=l.lanes|l.childLanes;if(tv(n,v),n===on&&(Yt=on=null,dn=0),(l.subtreeFlags&2064)===0&&(l.flags&2064)===0||Sl||(Sl=!0,Xm(xt,function(){return Fs(),null})),v=(l.flags&15990)!==0,(l.subtreeFlags&15990)!==0||v){v=Qn.transition,Qn.transition=null;var A=Ct;Ct=1;var B=Mt;Mt|=4,Jc.current=null,my(n,l),Lm(l,n),Hv(ac),Fa=!!oc,ac=oc=null,n.current=l,gy(l),Ye(),Mt=B,Ct=A,Qn.transition=v}else n.current=l;if(Sl&&(Sl=!1,mr=n,Ml=p),v=n.pendingLanes,v===0&&(pr=null),Je(l.stateNode),In(n,Ie()),r!==null)for(f=n.onRecoverableError,l=0;l<r.length;l++)p=r[l],f(p.value,{componentStack:p.stack,digest:p.digest});if(xl)throw xl=!1,n=tf,tf=null,n;return(Ml&1)!==0&&n.tag!==0&&Fs(),v=n.pendingLanes,(v&1)!==0?n===nf?na++:(na=0,nf=n):na=0,cr(),null}function Fs(){if(mr!==null){var n=Pd(Ml),r=Qn.transition,l=Ct;try{if(Qn.transition=null,Ct=16>n?16:n,mr===null)var f=!1;else{if(n=mr,mr=null,Ml=0,(Mt&6)!==0)throw Error(t(331));var p=Mt;for(Mt|=4,je=n.current;je!==null;){var v=je,A=v.child;if((je.flags&16)!==0){var B=v.deletions;if(B!==null){for(var V=0;V<B.length;V++){var ue=B[V];for(je=ue;je!==null;){var Ee=je;switch(Ee.tag){case 0:case 11:case 15:Qo(8,Ee,v)}var Ce=Ee.child;if(Ce!==null)Ce.return=Ee,je=Ce;else for(;je!==null;){Ee=je;var Me=Ee.sibling,Ge=Ee.return;if(Am(Ee),Ee===ue){je=null;break}if(Me!==null){Me.return=Ge,je=Me;break}je=Ge}}}var qe=v.alternate;if(qe!==null){var Ke=qe.child;if(Ke!==null){qe.child=null;do{var jt=Ke.sibling;Ke.sibling=null,Ke=jt}while(Ke!==null)}}je=v}}if((v.subtreeFlags&2064)!==0&&A!==null)A.return=v,je=A;else e:for(;je!==null;){if(v=je,(v.flags&2048)!==0)switch(v.tag){case 0:case 11:case 15:Qo(9,v,v.return)}var J=v.sibling;if(J!==null){J.return=v.return,je=J;break e}je=v.return}}var j=n.current;for(je=j;je!==null;){A=je;var Q=A.child;if((A.subtreeFlags&2064)!==0&&Q!==null)Q.return=A,je=Q;else e:for(A=j;je!==null;){if(B=je,(B.flags&2048)!==0)try{switch(B.tag){case 0:case 11:case 15:_l(9,B)}}catch($e){Xt(B,B.return,$e)}if(B===A){je=null;break e}var be=B.sibling;if(be!==null){be.return=B.return,je=be;break e}je=B.return}}if(Mt=p,cr(),ht&&typeof ht.onPostCommitFiberRoot=="function")try{ht.onPostCommitFiberRoot(nn,n)}catch{}f=!0}return f}finally{Ct=l,Qn.transition=r}}return!1}function Vm(n,r,l){r=Ds(l,r),r=om(n,r,1),n=hr(n,r,1),r=wn(),n!==null&&(Ao(n,1,r),In(n,r))}function Xt(n,r,l){if(n.tag===3)Vm(n,n,l);else for(;r!==null;){if(r.tag===3){Vm(r,n,l);break}else if(r.tag===1){var f=r.stateNode;if(typeof r.type.getDerivedStateFromError=="function"||typeof f.componentDidCatch=="function"&&(pr===null||!pr.has(f))){n=Ds(l,n),n=am(r,n,1),r=hr(r,n,1),n=wn(),r!==null&&(Ao(r,1,n),In(r,n));break}}r=r.return}}function My(n,r,l){var f=n.pingCache;f!==null&&f.delete(r),r=wn(),n.pingedLanes|=n.suspendedLanes&l,on===n&&(dn&l)===l&&(Zt===4||Zt===3&&(dn&130023424)===dn&&500>Ie()-ef?jr(n,0):Qc|=l),In(n,r)}function Gm(n,r){r===0&&((n.mode&1)===0?r=1:(r=er,er<<=1,(er&130023424)===0&&(er=4194304)));var l=wn();n=Ui(n,r),n!==null&&(Ao(n,r,l),In(n,l))}function Ey(n){var r=n.memoizedState,l=0;r!==null&&(l=r.retryLane),Gm(n,l)}function wy(n,r){var l=0;switch(n.tag){case 13:var f=n.stateNode,p=n.memoizedState;p!==null&&(l=p.retryLane);break;case 19:f=n.stateNode;break;default:throw Error(t(314))}f!==null&&f.delete(r),Gm(n,l)}var Wm;Wm=function(n,r,l){if(n!==null)if(n.memoizedProps!==r.pendingProps||bn.current)Dn=!0;else{if((n.lanes&l)===0&&(r.flags&128)===0)return Dn=!1,fy(n,r,l);Dn=(n.flags&131072)!==0}else Dn=!1,Bt&&(r.flags&1048576)!==0&&Ep(r,el,r.index);switch(r.lanes=0,r.tag){case 2:var f=r.type;ml(n,r),n=r.pendingProps;var p=ws(r,_n.current);bs(r,l),p=Lc(null,r,f,n,p,l);var v=Dc();return r.flags|=1,typeof p=="object"&&p!==null&&typeof p.render=="function"&&p.$$typeof===void 0?(r.tag=1,r.memoizedState=null,r.updateQueue=null,Ln(f)?(v=!0,Za(r)):v=!1,r.memoizedState=p.state!==null&&p.state!==void 0?p.state:null,wc(r),p.updater=dl,r.stateNode=p,p._reactInternals=r,Bc(r,f,n,l),r=Vc(null,r,f,!0,v,l)):(r.tag=0,Bt&&v&&pc(r),En(null,r,p,l),r=r.child),r;case 16:f=r.elementType;e:{switch(ml(n,r),n=r.pendingProps,p=f._init,f=p(f._payload),r.type=f,p=r.tag=Ay(f),n=li(f,n),p){case 0:r=Hc(null,r,f,n,l);break e;case 1:r=gm(null,r,f,n,l);break e;case 11:r=fm(null,r,f,n,l);break e;case 14:r=hm(null,r,f,li(f.type,n),l);break e}throw Error(t(306,f,""))}return r;case 0:return f=r.type,p=r.pendingProps,p=r.elementType===f?p:li(f,p),Hc(n,r,f,p,l);case 1:return f=r.type,p=r.pendingProps,p=r.elementType===f?p:li(f,p),gm(n,r,f,p,l);case 3:e:{if(_m(r),n===null)throw Error(t(387));f=r.pendingProps,v=r.memoizedState,p=v.element,Dp(n,r),ol(r,f,null,l);var A=r.memoizedState;if(f=A.element,v.isDehydrated)if(v={element:f,isDehydrated:!1,cache:A.cache,pendingSuspenseBoundaries:A.pendingSuspenseBoundaries,transitions:A.transitions},r.updateQueue.baseState=v,r.memoizedState=v,r.flags&256){p=Ds(Error(t(423)),r),r=vm(n,r,f,l,p);break e}else if(f!==p){p=Ds(Error(t(424)),r),r=vm(n,r,f,l,p);break e}else for(Xn=ar(r.stateNode.containerInfo.firstChild),Wn=r,Bt=!0,ai=null,l=bp(r,null,f,l),r.child=l;l;)l.flags=l.flags&-3|4096,l=l.sibling;else{if(Cs(),f===p){r=Oi(n,r,l);break e}En(n,r,f,l)}r=r.child}return r;case 5:return Up(r),n===null&&_c(r),f=r.type,p=r.pendingProps,v=n!==null?n.memoizedProps:null,A=p.children,lc(f,p)?A=null:v!==null&&lc(f,v)&&(r.flags|=32),mm(n,r),En(n,r,A,l),r.child;case 6:return n===null&&_c(r),null;case 13:return ym(n,r,l);case 4:return Tc(r,r.stateNode.containerInfo),f=r.pendingProps,n===null?r.child=Rs(r,null,f,l):En(n,r,f,l),r.child;case 11:return f=r.type,p=r.pendingProps,p=r.elementType===f?p:li(f,p),fm(n,r,f,p,l);case 7:return En(n,r,r.pendingProps,l),r.child;case 8:return En(n,r,r.pendingProps.children,l),r.child;case 12:return En(n,r,r.pendingProps.children,l),r.child;case 10:e:{if(f=r.type._context,p=r.pendingProps,v=r.memoizedProps,A=p.value,Dt(il,f._currentValue),f._currentValue=A,v!==null)if(oi(v.value,A)){if(v.children===p.children&&!bn.current){r=Oi(n,r,l);break e}}else for(v=r.child,v!==null&&(v.return=r);v!==null;){var B=v.dependencies;if(B!==null){A=v.child;for(var V=B.firstContext;V!==null;){if(V.context===f){if(v.tag===1){V=Fi(-1,l&-l),V.tag=2;var ue=v.updateQueue;if(ue!==null){ue=ue.shared;var Ee=ue.pending;Ee===null?V.next=V:(V.next=Ee.next,Ee.next=V),ue.pending=V}}v.lanes|=l,V=v.alternate,V!==null&&(V.lanes|=l),Mc(v.return,l,r),B.lanes|=l;break}V=V.next}}else if(v.tag===10)A=v.type===r.type?null:v.child;else if(v.tag===18){if(A=v.return,A===null)throw Error(t(341));A.lanes|=l,B=A.alternate,B!==null&&(B.lanes|=l),Mc(A,l,r),A=v.sibling}else A=v.child;if(A!==null)A.return=v;else for(A=v;A!==null;){if(A===r){A=null;break}if(v=A.sibling,v!==null){v.return=A.return,A=v;break}A=A.return}v=A}En(n,r,p.children,l),r=r.child}return r;case 9:return p=r.type,f=r.pendingProps.children,bs(r,l),p=Zn(p),f=f(p),r.flags|=1,En(n,r,f,l),r.child;case 14:return f=r.type,p=li(f,r.pendingProps),p=li(f.type,p),hm(n,r,f,p,l);case 15:return dm(n,r,r.type,r.pendingProps,l);case 17:return f=r.type,p=r.pendingProps,p=r.elementType===f?p:li(f,p),ml(n,r),r.tag=1,Ln(f)?(n=!0,Za(r)):n=!1,bs(r,l),rm(r,f,p),Bc(r,f,p,l),Vc(null,r,f,!0,n,l);case 19:return Sm(n,r,l);case 22:return pm(n,r,l)}throw Error(t(156,r.tag))};function Xm(n,r){return $(n,r)}function Ty(n,r,l,f){this.tag=n,this.key=l,this.sibling=this.child=this.return=this.stateNode=this.type=this.elementType=null,this.index=0,this.ref=null,this.pendingProps=r,this.dependencies=this.memoizedState=this.updateQueue=this.memoizedProps=null,this.mode=f,this.subtreeFlags=this.flags=0,this.deletions=null,this.childLanes=this.lanes=0,this.alternate=null}function ei(n,r,l,f){return new Ty(n,r,l,f)}function uf(n){return n=n.prototype,!(!n||!n.isReactComponent)}function Ay(n){if(typeof n=="function")return uf(n)?1:0;if(n!=null){if(n=n.$$typeof,n===X)return 11;if(n===ae)return 14}return 2}function vr(n,r){var l=n.alternate;return l===null?(l=ei(n.tag,r,n.key,n.mode),l.elementType=n.elementType,l.type=n.type,l.stateNode=n.stateNode,l.alternate=n,n.alternate=l):(l.pendingProps=r,l.type=n.type,l.flags=0,l.subtreeFlags=0,l.deletions=null),l.flags=n.flags&14680064,l.childLanes=n.childLanes,l.lanes=n.lanes,l.child=n.child,l.memoizedProps=n.memoizedProps,l.memoizedState=n.memoizedState,l.updateQueue=n.updateQueue,r=n.dependencies,l.dependencies=r===null?null:{lanes:r.lanes,firstContext:r.firstContext},l.sibling=n.sibling,l.index=n.index,l.ref=n.ref,l}function Al(n,r,l,f,p,v){var A=2;if(f=n,typeof n=="function")uf(n)&&(A=1);else if(typeof n=="string")A=5;else e:switch(n){case N:return Yr(l.children,p,v,r);case z:A=8,p|=8;break;case C:return n=ei(12,l,r,p|2),n.elementType=C,n.lanes=v,n;case k:return n=ei(13,l,r,p),n.elementType=k,n.lanes=v,n;case Y:return n=ei(19,l,r,p),n.elementType=Y,n.lanes=v,n;case ce:return Cl(l,p,v,r);default:if(typeof n=="object"&&n!==null)switch(n.$$typeof){case w:A=10;break e;case U:A=9;break e;case X:A=11;break e;case ae:A=14;break e;case te:A=16,f=null;break e}throw Error(t(130,n==null?n:typeof n,""))}return r=ei(A,l,r,p),r.elementType=n,r.type=f,r.lanes=v,r}function Yr(n,r,l,f){return n=ei(7,n,f,r),n.lanes=l,n}function Cl(n,r,l,f){return n=ei(22,n,f,r),n.elementType=ce,n.lanes=l,n.stateNode={isHidden:!1},n}function cf(n,r,l){return n=ei(6,n,null,r),n.lanes=l,n}function ff(n,r,l){return r=ei(4,n.children!==null?n.children:[],n.key,r),r.lanes=l,r.stateNode={containerInfo:n.containerInfo,pendingChildren:null,implementation:n.implementation},r}function Cy(n,r,l,f,p){this.tag=r,this.containerInfo=n,this.finishedWork=this.pingCache=this.current=this.pendingChildren=null,this.timeoutHandle=-1,this.callbackNode=this.pendingContext=this.context=null,this.callbackPriority=0,this.eventTimes=ps(0),this.expirationTimes=ps(-1),this.entangledLanes=this.finishedLanes=this.mutableReadLanes=this.expiredLanes=this.pingedLanes=this.suspendedLanes=this.pendingLanes=0,this.entanglements=ps(0),this.identifierPrefix=f,this.onRecoverableError=p,this.mutableSourceEagerHydrationData=null}function hf(n,r,l,f,p,v,A,B,V){return n=new Cy(n,r,l,B,V),r===1?(r=1,v===!0&&(r|=8)):r=0,v=ei(3,null,null,r),n.current=v,v.stateNode=n,v.memoizedState={element:f,isDehydrated:l,cache:null,transitions:null,pendingSuspenseBoundaries:null},wc(v),n}function Ry(n,r,l){var f=3<arguments.length&&arguments[3]!==void 0?arguments[3]:null;return{$$typeof:D,key:f==null?null:""+f,children:n,containerInfo:r,implementation:l}}function jm(n){if(!n)return ur;n=n._reactInternals;e:{if(bi(n)!==n||n.tag!==1)throw Error(t(170));var r=n;do{switch(r.tag){case 3:r=r.stateNode.context;break e;case 1:if(Ln(r.type)){r=r.stateNode.__reactInternalMemoizedMergedChildContext;break e}}r=r.return}while(r!==null);throw Error(t(171))}if(n.tag===1){var l=n.type;if(Ln(l))return xp(n,l,r)}return r}function qm(n,r,l,f,p,v,A,B,V){return n=hf(l,f,!0,n,p,v,A,B,V),n.context=jm(null),l=n.current,f=wn(),p=gr(l),v=Fi(f,p),v.callback=r??null,hr(l,v,p),n.current.lanes=p,Ao(n,p,f),In(n,f),n}function Rl(n,r,l,f){var p=r.current,v=wn(),A=gr(p);return l=jm(l),r.context===null?r.context=l:r.pendingContext=l,r=Fi(v,A),r.payload={element:n},f=f===void 0?null:f,f!==null&&(r.callback=f),n=hr(p,r,A),n!==null&&(fi(n,p,A,v),sl(n,p,A)),A}function Pl(n){if(n=n.current,!n.child)return null;switch(n.child.tag){case 5:return n.child.stateNode;default:return n.child.stateNode}}function Ym(n,r){if(n=n.memoizedState,n!==null&&n.dehydrated!==null){var l=n.retryLane;n.retryLane=l!==0&&l<r?l:r}}function df(n,r){Ym(n,r),(n=n.alternate)&&Ym(n,r)}function Py(){return null}var Km=typeof reportError=="function"?reportError:function(n){console.error(n)};function pf(n){this._internalRoot=n}bl.prototype.render=pf.prototype.render=function(n){var r=this._internalRoot;if(r===null)throw Error(t(409));Rl(n,r,null,null)},bl.prototype.unmount=pf.prototype.unmount=function(){var n=this._internalRoot;if(n!==null){this._internalRoot=null;var r=n.containerInfo;Xr(function(){Rl(null,n,null,null)}),r[Li]=null}};function bl(n){this._internalRoot=n}bl.prototype.unstable_scheduleHydration=function(n){if(n){var r=Dd();n={blockedOn:null,target:n,priority:r};for(var l=0;l<rr.length&&r!==0&&r<rr[l].priority;l++);rr.splice(l,0,n),l===0&&Ud(n)}};function mf(n){return!(!n||n.nodeType!==1&&n.nodeType!==9&&n.nodeType!==11)}function Ll(n){return!(!n||n.nodeType!==1&&n.nodeType!==9&&n.nodeType!==11&&(n.nodeType!==8||n.nodeValue!==" react-mount-point-unstable "))}function $m(){}function by(n,r,l,f,p){if(p){if(typeof f=="function"){var v=f;f=function(){var ue=Pl(A);v.call(ue)}}var A=qm(r,f,n,0,null,!1,!1,"",$m);return n._reactRootContainer=A,n[Li]=A.current,ko(n.nodeType===8?n.parentNode:n),Xr(),A}for(;p=n.lastChild;)n.removeChild(p);if(typeof f=="function"){var B=f;f=function(){var ue=Pl(V);B.call(ue)}}var V=hf(n,0,!1,null,null,!1,!1,"",$m);return n._reactRootContainer=V,n[Li]=V.current,ko(n.nodeType===8?n.parentNode:n),Xr(function(){Rl(r,V,l,f)}),V}function Dl(n,r,l,f,p){var v=l._reactRootContainer;if(v){var A=v;if(typeof p=="function"){var B=p;p=function(){var V=Pl(A);B.call(V)}}Rl(r,A,n,p)}else A=by(l,r,n,p,f);return Pl(A)}bd=function(n){switch(n.tag){case 3:var r=n.stateNode;if(r.current.memoizedState.isDehydrated){var l=Lt(r.pendingLanes);l!==0&&(Bu(r,l|1),In(r,Ie()),(Mt&6)===0&&(Us=Ie()+500,cr()))}break;case 13:Xr(function(){var f=Ui(n,1);if(f!==null){var p=wn();fi(f,n,1,p)}}),df(n,1)}},zu=function(n){if(n.tag===13){var r=Ui(n,134217728);if(r!==null){var l=wn();fi(r,n,134217728,l)}df(n,134217728)}},Ld=function(n){if(n.tag===13){var r=gr(n),l=Ui(n,r);if(l!==null){var f=wn();fi(l,n,r,f)}df(n,r)}},Dd=function(){return Ct},Nd=function(n,r){var l=Ct;try{return Ct=n,r()}finally{Ct=l}},me=function(n,r,l){switch(r){case"input":if(et(n,l),r=l.name,l.type==="radio"&&r!=null){for(l=n;l.parentNode;)l=l.parentNode;for(l=l.querySelectorAll("input[name="+JSON.stringify(""+r)+'][type="radio"]'),r=0;r<l.length;r++){var f=l[r];if(f!==n&&f.form===n.form){var p=Ka(f);if(!p)throw Error(t(90));I(f),et(f,p)}}}break;case"textarea":_e(n,l);break;case"select":r=l.value,r!=null&&R(n,!!l.multiple,r,!1)}},qt=of,St=Xr;var Ly={usingClientEntryPoint:!1,Events:[Go,Ms,Ka,vt,Ht,of]},ia={findFiberByHostInstance:Or,bundleType:0,version:"18.3.1",rendererPackageName:"react-dom"},Dy={bundleType:ia.bundleType,version:ia.version,rendererPackageName:ia.rendererPackageName,rendererConfig:ia.rendererConfig,overrideHookState:null,overrideHookStateDeletePath:null,overrideHookStateRenamePath:null,overrideProps:null,overridePropsDeletePath:null,overridePropsRenamePath:null,setErrorHandler:null,setSuspenseHandler:null,scheduleUpdate:null,currentDispatcherRef:P.ReactCurrentDispatcher,findHostInstanceByFiber:function(n){return n=se(n),n===null?null:n.stateNode},findFiberByHostInstance:ia.findFiberByHostInstance||Py,findHostInstancesForRefresh:null,scheduleRefresh:null,scheduleRoot:null,setRefreshHandler:null,getCurrentFiber:null,reconcilerVersion:"18.3.1-next-f1338f8080-20240426"};if(typeof __REACT_DEVTOOLS_GLOBAL_HOOK__<"u"){var Nl=__REACT_DEVTOOLS_GLOBAL_HOOK__;if(!Nl.isDisabled&&Nl.supportsFiber)try{nn=Nl.inject(Dy),ht=Nl}catch{}}return Un.__SECRET_INTERNALS_DO_NOT_USE_OR_YOU_WILL_BE_FIRED=Ly,Un.createPortal=function(n,r){var l=2<arguments.length&&arguments[2]!==void 0?arguments[2]:null;if(!mf(r))throw Error(t(200));return Ry(n,r,null,l)},Un.createRoot=function(n,r){if(!mf(n))throw Error(t(299));var l=!1,f="",p=Km;return r!=null&&(r.unstable_strictMode===!0&&(l=!0),r.identifierPrefix!==void 0&&(f=r.identifierPrefix),r.onRecoverableError!==void 0&&(p=r.onRecoverableError)),r=hf(n,1,!1,null,null,l,!1,f,p),n[Li]=r.current,ko(n.nodeType===8?n.parentNode:n),new pf(r)},Un.findDOMNode=function(n){if(n==null)return null;if(n.nodeType===1)return n;var r=n._reactInternals;if(r===void 0)throw typeof n.render=="function"?Error(t(188)):(n=Object.keys(n).join(","),Error(t(268,n)));return n=se(r),n=n===null?null:n.stateNode,n},Un.flushSync=function(n){return Xr(n)},Un.hydrate=function(n,r,l){if(!Ll(r))throw Error(t(200));return Dl(null,n,r,!0,l)},Un.hydrateRoot=function(n,r,l){if(!mf(n))throw Error(t(405));var f=l!=null&&l.hydratedSources||null,p=!1,v="",A=Km;if(l!=null&&(l.unstable_strictMode===!0&&(p=!0),l.identifierPrefix!==void 0&&(v=l.identifierPrefix),l.onRecoverableError!==void 0&&(A=l.onRecoverableError)),r=qm(r,null,n,1,l??null,p,!1,v,A),n[Li]=r.current,ko(n),f)for(n=0;n<f.length;n++)l=f[n],p=l._getVersion,p=p(l._source),r.mutableSourceEagerHydrationData==null?r.mutableSourceEagerHydrationData=[l,p]:r.mutableSourceEagerHydrationData.push(l,p);return new bl(r)},Un.render=function(n,r,l){if(!Ll(r))throw Error(t(200));return Dl(null,n,r,!1,l)},Un.unmountComponentAtNode=function(n){if(!Ll(n))throw Error(t(40));return n._reactRootContainer?(Xr(function(){Dl(null,null,n,!1,function(){n._reactRootContainer=null,n[Li]=null})}),!0):!1},Un.unstable_batchedUpdates=of,Un.unstable_renderSubtreeIntoContainer=function(n,r,l,f){if(!Ll(l))throw Error(t(200));if(n==null||n._reactInternals===void 0)throw Error(t(38));return Dl(n,r,l,!1,f)},Un.version="18.3.1-next-f1338f8080-20240426",Un}var rg;function Hy(){if(rg)return vf.exports;rg=1;function s(){if(!(typeof __REACT_DEVTOOLS_GLOBAL_HOOK__>"u"||typeof __REACT_DEVTOOLS_GLOBAL_HOOK__.checkDCE!="function"))try{__REACT_DEVTOOLS_GLOBAL_HOOK__.checkDCE(s)}catch(e){console.error(e)}}return s(),vf.exports=ky(),vf.exports}var sg;function Vy(){if(sg)return Il;sg=1;var s=Hy();return Il.createRoot=s.createRoot,Il.hydrateRoot=s.hydrateRoot,Il}var Gy=Vy();const Wy=cd(Gy);var Os={},Sf,og;function Xy(){return og||(og=1,Sf=function(){return typeof Promise=="function"&&Promise.prototype&&Promise.prototype.then}),Sf}var Mf={},xr={},ag;function cs(){if(ag)return xr;ag=1;let s;const e=[0,26,44,70,100,134,172,196,242,292,346,404,466,532,581,655,733,815,901,991,1085,1156,1258,1364,1474,1588,1706,1828,1921,2051,2185,2323,2465,2611,2761,2876,3034,3196,3362,3532,3706];return xr.getSymbolSize=function(i){if(!i)throw new Error('"version" cannot be null or undefined');if(i<1||i>40)throw new Error('"version" should be in range from 1 to 40');return i*4+17},xr.getSymbolTotalCodewords=function(i){return e[i]},xr.getBCHDigit=function(t){let i=0;for(;t!==0;)i++,t>>>=1;return i},xr.setToSJISFunction=function(i){if(typeof i!="function")throw new Error('"toSJISFunc" is not a valid function.');s=i},xr.isKanjiModeEnabled=function(){return typeof s<"u"},xr.toSJIS=function(i){return s(i)},xr}var Ef={},lg;function hd(){return lg||(lg=1,(function(s){s.L={bit:1},s.M={bit:0},s.Q={bit:3},s.H={bit:2};function e(t){if(typeof t!="string")throw new Error("Param is not a string");switch(t.toLowerCase()){case"l":case"low":return s.L;case"m":case"medium":return s.M;case"q":case"quartile":return s.Q;case"h":case"high":return s.H;default:throw new Error("Unknown EC Level: "+t)}}s.isValid=function(i){return i&&typeof i.bit<"u"&&i.bit>=0&&i.bit<4},s.from=function(i,o){if(s.isValid(i))return i;try{return e(i)}catch{return o}}})(Ef)),Ef}var wf,ug;function jy(){if(ug)return wf;ug=1;function s(){this.buffer=[],this.length=0}return s.prototype={get:function(e){const t=Math.floor(e/8);return(this.buffer[t]>>>7-e%8&1)===1},put:function(e,t){for(let i=0;i<t;i++)this.putBit((e>>>t-i-1&1)===1)},getLengthInBits:function(){return this.length},putBit:function(e){const t=Math.floor(this.length/8);this.buffer.length<=t&&this.buffer.push(0),e&&(this.buffer[t]|=128>>>this.length%8),this.length++}},wf=s,wf}var Tf,cg;function qy(){if(cg)return Tf;cg=1;function s(e){if(!e||e<1)throw new Error("BitMatrix size must be defined and greater than 0");this.size=e,this.data=new Uint8Array(e*e),this.reservedBit=new Uint8Array(e*e)}return s.prototype.set=function(e,t,i,o){const a=e*this.size+t;this.data[a]=i,o&&(this.reservedBit[a]=!0)},s.prototype.get=function(e,t){return this.data[e*this.size+t]},s.prototype.xor=function(e,t,i){this.data[e*this.size+t]^=i},s.prototype.isReserved=function(e,t){return this.reservedBit[e*this.size+t]},Tf=s,Tf}var Af={},fg;function Yy(){return fg||(fg=1,(function(s){const e=cs().getSymbolSize;s.getRowColCoords=function(i){if(i===1)return[];const o=Math.floor(i/7)+2,a=e(i),u=a===145?26:Math.ceil((a-13)/(2*o-2))*2,c=[a-7];for(let h=1;h<o-1;h++)c[h]=c[h-1]-u;return c.push(6),c.reverse()},s.getPositions=function(i){const o=[],a=s.getRowColCoords(i),u=a.length;for(let c=0;c<u;c++)for(let h=0;h<u;h++)c===0&&h===0||c===0&&h===u-1||c===u-1&&h===0||o.push([a[c],a[h]]);return o}})(Af)),Af}var Cf={},hg;function Ky(){if(hg)return Cf;hg=1;const s=cs().getSymbolSize,e=7;return Cf.getPositions=function(i){const o=s(i);return[[0,0],[o-e,0],[0,o-e]]},Cf}var Rf={},dg;function $y(){return dg||(dg=1,(function(s){s.Patterns={PATTERN000:0,PATTERN001:1,PATTERN010:2,PATTERN011:3,PATTERN100:4,PATTERN101:5,PATTERN110:6,PATTERN111:7};const e={N1:3,N2:3,N3:40,N4:10};s.isValid=function(o){return o!=null&&o!==""&&!isNaN(o)&&o>=0&&o<=7},s.from=function(o){return s.isValid(o)?parseInt(o,10):void 0},s.getPenaltyN1=function(o){const a=o.size;let u=0,c=0,h=0,d=null,m=null;for(let _=0;_<a;_++){c=h=0,d=m=null;for(let g=0;g<a;g++){let S=o.get(_,g);S===d?c++:(c>=5&&(u+=e.N1+(c-5)),d=S,c=1),S=o.get(g,_),S===m?h++:(h>=5&&(u+=e.N1+(h-5)),m=S,h=1)}c>=5&&(u+=e.N1+(c-5)),h>=5&&(u+=e.N1+(h-5))}return u},s.getPenaltyN2=function(o){const a=o.size;let u=0;for(let c=0;c<a-1;c++)for(let h=0;h<a-1;h++){const d=o.get(c,h)+o.get(c,h+1)+o.get(c+1,h)+o.get(c+1,h+1);(d===4||d===0)&&u++}return u*e.N2},s.getPenaltyN3=function(o){const a=o.size;let u=0,c=0,h=0;for(let d=0;d<a;d++){c=h=0;for(let m=0;m<a;m++)c=c<<1&2047|o.get(d,m),m>=10&&(c===1488||c===93)&&u++,h=h<<1&2047|o.get(m,d),m>=10&&(h===1488||h===93)&&u++}return u*e.N3},s.getPenaltyN4=function(o){let a=0;const u=o.data.length;for(let h=0;h<u;h++)a+=o.data[h];return Math.abs(Math.ceil(a*100/u/5)-10)*e.N4};function t(i,o,a){switch(i){case s.Patterns.PATTERN000:return(o+a)%2===0;case s.Patterns.PATTERN001:return o%2===0;case s.Patterns.PATTERN010:return a%3===0;case s.Patterns.PATTERN011:return(o+a)%3===0;case s.Patterns.PATTERN100:return(Math.floor(o/2)+Math.floor(a/3))%2===0;case s.Patterns.PATTERN101:return o*a%2+o*a%3===0;case s.Patterns.PATTERN110:return(o*a%2+o*a%3)%2===0;case s.Patterns.PATTERN111:return(o*a%3+(o+a)%2)%2===0;default:throw new Error("bad maskPattern:"+i)}}s.applyMask=function(o,a){const u=a.size;for(let c=0;c<u;c++)for(let h=0;h<u;h++)a.isReserved(h,c)||a.xor(h,c,t(o,h,c))},s.getBestMask=function(o,a){const u=Object.keys(s.Patterns).length;let c=0,h=1/0;for(let d=0;d<u;d++){a(d),s.applyMask(d,o);const m=s.getPenaltyN1(o)+s.getPenaltyN2(o)+s.getPenaltyN3(o)+s.getPenaltyN4(o);s.applyMask(d,o),m<h&&(h=m,c=d)}return c}})(Rf)),Rf}var Ul={},pg;function r_(){if(pg)return Ul;pg=1;const s=hd(),e=[1,1,1,1,1,1,1,1,1,1,2,2,1,2,2,4,1,2,4,4,2,4,4,4,2,4,6,5,2,4,6,6,2,5,8,8,4,5,8,8,4,5,8,11,4,8,10,11,4,9,12,16,4,9,16,16,6,10,12,18,6,10,17,16,6,11,16,19,6,13,18,21,7,14,21,25,8,16,20,25,8,17,23,25,9,17,23,34,9,18,25,30,10,20,27,32,12,21,29,35,12,23,34,37,12,25,34,40,13,26,35,42,14,28,38,45,15,29,40,48,16,31,43,51,17,33,45,54,18,35,48,57,19,37,51,60,19,38,53,63,20,40,56,66,21,43,59,70,22,45,62,74,24,47,65,77,25,49,68,81],t=[7,10,13,17,10,16,22,28,15,26,36,44,20,36,52,64,26,48,72,88,36,64,96,112,40,72,108,130,48,88,132,156,60,110,160,192,72,130,192,224,80,150,224,264,96,176,260,308,104,198,288,352,120,216,320,384,132,240,360,432,144,280,408,480,168,308,448,532,180,338,504,588,196,364,546,650,224,416,600,700,224,442,644,750,252,476,690,816,270,504,750,900,300,560,810,960,312,588,870,1050,336,644,952,1110,360,700,1020,1200,390,728,1050,1260,420,784,1140,1350,450,812,1200,1440,480,868,1290,1530,510,924,1350,1620,540,980,1440,1710,570,1036,1530,1800,570,1064,1590,1890,600,1120,1680,1980,630,1204,1770,2100,660,1260,1860,2220,720,1316,1950,2310,750,1372,2040,2430];return Ul.getBlocksCount=function(o,a){switch(a){case s.L:return e[(o-1)*4+0];case s.M:return e[(o-1)*4+1];case s.Q:return e[(o-1)*4+2];case s.H:return e[(o-1)*4+3];default:return}},Ul.getTotalCodewordsCount=function(o,a){switch(a){case s.L:return t[(o-1)*4+0];case s.M:return t[(o-1)*4+1];case s.Q:return t[(o-1)*4+2];case s.H:return t[(o-1)*4+3];default:return}},Ul}var Pf={},sa={},mg;function Zy(){if(mg)return sa;mg=1;const s=new Uint8Array(512),e=new Uint8Array(256);return(function(){let i=1;for(let o=0;o<255;o++)s[o]=i,e[i]=o,i<<=1,i&256&&(i^=285);for(let o=255;o<512;o++)s[o]=s[o-255]})(),sa.log=function(i){if(i<1)throw new Error("log("+i+")");return e[i]},sa.exp=function(i){return s[i]},sa.mul=function(i,o){return i===0||o===0?0:s[e[i]+e[o]]},sa}var gg;function Jy(){return gg||(gg=1,(function(s){const e=Zy();s.mul=function(i,o){const a=new Uint8Array(i.length+o.length-1);for(let u=0;u<i.length;u++)for(let c=0;c<o.length;c++)a[u+c]^=e.mul(i[u],o[c]);return a},s.mod=function(i,o){let a=new Uint8Array(i);for(;a.length-o.length>=0;){const u=a[0];for(let h=0;h<o.length;h++)a[h]^=e.mul(o[h],u);let c=0;for(;c<a.length&&a[c]===0;)c++;a=a.slice(c)}return a},s.generateECPolynomial=function(i){let o=new Uint8Array([1]);for(let a=0;a<i;a++)o=s.mul(o,new Uint8Array([1,e.exp(a)]));return o}})(Pf)),Pf}var bf,_g;function Qy(){if(_g)return bf;_g=1;const s=Jy();function e(t){this.genPoly=void 0,this.degree=t,this.degree&&this.initialize(this.degree)}return e.prototype.initialize=function(i){this.degree=i,this.genPoly=s.generateECPolynomial(this.degree)},e.prototype.encode=function(i){if(!this.genPoly)throw new Error("Encoder not initialized");const o=new Uint8Array(i.length+this.degree);o.set(i);const a=s.mod(o,this.genPoly),u=this.degree-a.length;if(u>0){const c=new Uint8Array(this.degree);return c.set(a,u),c}return a},bf=e,bf}var Lf={},Df={},Nf={},vg;function s_(){return vg||(vg=1,Nf.isValid=function(e){return!isNaN(e)&&e>=1&&e<=40}),Nf}var Si={},yg;function o_(){if(yg)return Si;yg=1;const s="[0-9]+",e="[A-Z $%*+\\-./:]+";let t="(?:[u3000-u303F]|[u3040-u309F]|[u30A0-u30FF]|[uFF00-uFFEF]|[u4E00-u9FAF]|[u2605-u2606]|[u2190-u2195]|u203B|[u2010u2015u2018u2019u2025u2026u201Cu201Du2225u2260]|[u0391-u0451]|[u00A7u00A8u00B1u00B4u00D7u00F7])+";t=t.replace(/u/g,"\\u");const i="(?:(?![A-Z0-9 $%*+\\-./:]|"+t+`)(?:.|[\r
]))+`;Si.KANJI=new RegExp(t,"g"),Si.BYTE_KANJI=new RegExp("[^A-Z0-9 $%*+\\-./:]+","g"),Si.BYTE=new RegExp(i,"g"),Si.NUMERIC=new RegExp(s,"g"),Si.ALPHANUMERIC=new RegExp(e,"g");const o=new RegExp("^"+t+"$"),a=new RegExp("^"+s+"$"),u=new RegExp("^[A-Z0-9 $%*+\\-./:]+$");return Si.testKanji=function(h){return o.test(h)},Si.testNumeric=function(h){return a.test(h)},Si.testAlphanumeric=function(h){return u.test(h)},Si}var xg;function fs(){return xg||(xg=1,(function(s){const e=s_(),t=o_();s.NUMERIC={id:"Numeric",bit:1,ccBits:[10,12,14]},s.ALPHANUMERIC={id:"Alphanumeric",bit:2,ccBits:[9,11,13]},s.BYTE={id:"Byte",bit:4,ccBits:[8,16,16]},s.KANJI={id:"Kanji",bit:8,ccBits:[8,10,12]},s.MIXED={bit:-1},s.getCharCountIndicator=function(a,u){if(!a.ccBits)throw new Error("Invalid mode: "+a);if(!e.isValid(u))throw new Error("Invalid version: "+u);return u>=1&&u<10?a.ccBits[0]:u<27?a.ccBits[1]:a.ccBits[2]},s.getBestModeForData=function(a){return t.testNumeric(a)?s.NUMERIC:t.testAlphanumeric(a)?s.ALPHANUMERIC:t.testKanji(a)?s.KANJI:s.BYTE},s.toString=function(a){if(a&&a.id)return a.id;throw new Error("Invalid mode")},s.isValid=function(a){return a&&a.bit&&a.ccBits};function i(o){if(typeof o!="string")throw new Error("Param is not a string");switch(o.toLowerCase()){case"numeric":return s.NUMERIC;case"alphanumeric":return s.ALPHANUMERIC;case"kanji":return s.KANJI;case"byte":return s.BYTE;default:throw new Error("Unknown mode: "+o)}}s.from=function(a,u){if(s.isValid(a))return a;try{return i(a)}catch{return u}}})(Df)),Df}var Sg;function ex(){return Sg||(Sg=1,(function(s){const e=cs(),t=r_(),i=hd(),o=fs(),a=s_(),u=7973,c=e.getBCHDigit(u);function h(g,S,M){for(let E=1;E<=40;E++)if(S<=s.getCapacity(E,M,g))return E}function d(g,S){return o.getCharCountIndicator(g,S)+4}function m(g,S){let M=0;return g.forEach(function(E){const y=d(E.mode,S);M+=y+E.getBitsLength()}),M}function _(g,S){for(let M=1;M<=40;M++)if(m(g,M)<=s.getCapacity(M,S,o.MIXED))return M}s.from=function(S,M){return a.isValid(S)?parseInt(S,10):M},s.getCapacity=function(S,M,E){if(!a.isValid(S))throw new Error("Invalid QR Code version");typeof E>"u"&&(E=o.BYTE);const y=e.getSymbolTotalCodewords(S),x=t.getTotalCodewordsCount(S,M),b=(y-x)*8;if(E===o.MIXED)return b;const T=b-d(E,S);switch(E){case o.NUMERIC:return Math.floor(T/10*3);case o.ALPHANUMERIC:return Math.floor(T/11*2);case o.KANJI:return Math.floor(T/13);case o.BYTE:default:return Math.floor(T/8)}},s.getBestVersionForData=function(S,M){let E;const y=i.from(M,i.M);if(Array.isArray(S)){if(S.length>1)return _(S,y);if(S.length===0)return 1;E=S[0]}else E=S;return h(E.mode,E.getLength(),y)},s.getEncodedBits=function(S){if(!a.isValid(S)||S<7)throw new Error("Invalid QR Code version");let M=S<<12;for(;e.getBCHDigit(M)-c>=0;)M^=u<<e.getBCHDigit(M)-c;return S<<12|M}})(Lf)),Lf}var If={},Mg;function tx(){if(Mg)return If;Mg=1;const s=cs(),e=1335,t=21522,i=s.getBCHDigit(e);return If.getEncodedBits=function(a,u){const c=a.bit<<3|u;let h=c<<10;for(;s.getBCHDigit(h)-i>=0;)h^=e<<s.getBCHDigit(h)-i;return(c<<10|h)^t},If}var Uf={},Ff,Eg;function nx(){if(Eg)return Ff;Eg=1;const s=fs();function e(t){this.mode=s.NUMERIC,this.data=t.toString()}return e.getBitsLength=function(i){return 10*Math.floor(i/3)+(i%3?i%3*3+1:0)},e.prototype.getLength=function(){return this.data.length},e.prototype.getBitsLength=function(){return e.getBitsLength(this.data.length)},e.prototype.write=function(i){let o,a,u;for(o=0;o+3<=this.data.length;o+=3)a=this.data.substr(o,3),u=parseInt(a,10),i.put(u,10);const c=this.data.length-o;c>0&&(a=this.data.substr(o),u=parseInt(a,10),i.put(u,c*3+1))},Ff=e,Ff}var Of,wg;function ix(){if(wg)return Of;wg=1;const s=fs(),e=["0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"," ","$","%","*","+","-",".","/",":"];function t(i){this.mode=s.ALPHANUMERIC,this.data=i}return t.getBitsLength=function(o){return 11*Math.floor(o/2)+6*(o%2)},t.prototype.getLength=function(){return this.data.length},t.prototype.getBitsLength=function(){return t.getBitsLength(this.data.length)},t.prototype.write=function(o){let a;for(a=0;a+2<=this.data.length;a+=2){let u=e.indexOf(this.data[a])*45;u+=e.indexOf(this.data[a+1]),o.put(u,11)}this.data.length%2&&o.put(e.indexOf(this.data[a]),6)},Of=t,Of}var Bf,Tg;function rx(){if(Tg)return Bf;Tg=1;const s=fs();function e(t){this.mode=s.BYTE,typeof t=="string"?this.data=new TextEncoder().encode(t):this.data=new Uint8Array(t)}return e.getBitsLength=function(i){return i*8},e.prototype.getLength=function(){return this.data.length},e.prototype.getBitsLength=function(){return e.getBitsLength(this.data.length)},e.prototype.write=function(t){for(let i=0,o=this.data.length;i<o;i++)t.put(this.data[i],8)},Bf=e,Bf}var zf,Ag;function sx(){if(Ag)return zf;Ag=1;const s=fs(),e=cs();function t(i){this.mode=s.KANJI,this.data=i}return t.getBitsLength=function(o){return o*13},t.prototype.getLength=function(){return this.data.length},t.prototype.getBitsLength=function(){return t.getBitsLength(this.data.length)},t.prototype.write=function(i){let o;for(o=0;o<this.data.length;o++){let a=e.toSJIS(this.data[o]);if(a>=33088&&a<=40956)a-=33088;else if(a>=57408&&a<=60351)a-=49472;else throw new Error("Invalid SJIS character: "+this.data[o]+`
Make sure your charset is UTF-8`);a=(a>>>8&255)*192+(a&255),i.put(a,13)}},zf=t,zf}var kf={exports:{}},Cg;function ox(){return Cg||(Cg=1,(function(s){var e={single_source_shortest_paths:function(t,i,o){var a={},u={};u[i]=0;var c=e.PriorityQueue.make();c.push(i,0);for(var h,d,m,_,g,S,M,E,y;!c.empty();){h=c.pop(),d=h.value,_=h.cost,g=t[d]||{};for(m in g)g.hasOwnProperty(m)&&(S=g[m],M=_+S,E=u[m],y=typeof u[m]>"u",(y||E>M)&&(u[m]=M,c.push(m,M),a[m]=d))}if(typeof o<"u"&&typeof u[o]>"u"){var x=["Could not find a path from ",i," to ",o,"."].join("");throw new Error(x)}return a},extract_shortest_path_from_predecessor_list:function(t,i){for(var o=[],a=i;a;)o.push(a),t[a],a=t[a];return o.reverse(),o},find_path:function(t,i,o){var a=e.single_source_shortest_paths(t,i,o);return e.extract_shortest_path_from_predecessor_list(a,o)},PriorityQueue:{make:function(t){var i=e.PriorityQueue,o={},a;t=t||{};for(a in i)i.hasOwnProperty(a)&&(o[a]=i[a]);return o.queue=[],o.sorter=t.sorter||i.default_sorter,o},default_sorter:function(t,i){return t.cost-i.cost},push:function(t,i){var o={value:t,cost:i};this.queue.push(o),this.queue.sort(this.sorter)},pop:function(){return this.queue.shift()},empty:function(){return this.queue.length===0}}};s.exports=e})(kf)),kf.exports}var Rg;function ax(){return Rg||(Rg=1,(function(s){const e=fs(),t=nx(),i=ix(),o=rx(),a=sx(),u=o_(),c=cs(),h=ox();function d(x){return unescape(encodeURIComponent(x)).length}function m(x,b,T){const P=[];let H;for(;(H=x.exec(T))!==null;)P.push({data:H[0],index:H.index,mode:b,length:H[0].length});return P}function _(x){const b=m(u.NUMERIC,e.NUMERIC,x),T=m(u.ALPHANUMERIC,e.ALPHANUMERIC,x);let P,H;return c.isKanjiModeEnabled()?(P=m(u.BYTE,e.BYTE,x),H=m(u.KANJI,e.KANJI,x)):(P=m(u.BYTE_KANJI,e.BYTE,x),H=[]),b.concat(T,P,H).sort(function(N,z){return N.index-z.index}).map(function(N){return{data:N.data,mode:N.mode,length:N.length}})}function g(x,b){switch(b){case e.NUMERIC:return t.getBitsLength(x);case e.ALPHANUMERIC:return i.getBitsLength(x);case e.KANJI:return a.getBitsLength(x);case e.BYTE:return o.getBitsLength(x)}}function S(x){return x.reduce(function(b,T){const P=b.length-1>=0?b[b.length-1]:null;return P&&P.mode===T.mode?(b[b.length-1].data+=T.data,b):(b.push(T),b)},[])}function M(x){const b=[];for(let T=0;T<x.length;T++){const P=x[T];switch(P.mode){case e.NUMERIC:b.push([P,{data:P.data,mode:e.ALPHANUMERIC,length:P.length},{data:P.data,mode:e.BYTE,length:P.length}]);break;case e.ALPHANUMERIC:b.push([P,{data:P.data,mode:e.BYTE,length:P.length}]);break;case e.KANJI:b.push([P,{data:P.data,mode:e.BYTE,length:d(P.data)}]);break;case e.BYTE:b.push([{data:P.data,mode:e.BYTE,length:d(P.data)}])}}return b}function E(x,b){const T={},P={start:{}};let H=["start"];for(let D=0;D<x.length;D++){const N=x[D],z=[];for(let C=0;C<N.length;C++){const w=N[C],U=""+D+C;z.push(U),T[U]={node:w,lastCount:0},P[U]={};for(let X=0;X<H.length;X++){const k=H[X];T[k]&&T[k].node.mode===w.mode?(P[k][U]=g(T[k].lastCount+w.length,w.mode)-g(T[k].lastCount,w.mode),T[k].lastCount+=w.length):(T[k]&&(T[k].lastCount=w.length),P[k][U]=g(w.length,w.mode)+4+e.getCharCountIndicator(w.mode,b))}}H=z}for(let D=0;D<H.length;D++)P[H[D]].end=0;return{map:P,table:T}}function y(x,b){let T;const P=e.getBestModeForData(x);if(T=e.from(b,P),T!==e.BYTE&&T.bit<P.bit)throw new Error('"'+x+'" cannot be encoded with mode '+e.toString(T)+`.
 Suggested mode is: `+e.toString(P));switch(T===e.KANJI&&!c.isKanjiModeEnabled()&&(T=e.BYTE),T){case e.NUMERIC:return new t(x);case e.ALPHANUMERIC:return new i(x);case e.KANJI:return new a(x);case e.BYTE:return new o(x)}}s.fromArray=function(b){return b.reduce(function(T,P){return typeof P=="string"?T.push(y(P,null)):P.data&&T.push(y(P.data,P.mode)),T},[])},s.fromString=function(b,T){const P=_(b,c.isKanjiModeEnabled()),H=M(P),D=E(H,T),N=h.find_path(D.map,"start","end"),z=[];for(let C=1;C<N.length-1;C++)z.push(D.table[N[C]].node);return s.fromArray(S(z))},s.rawSplit=function(b){return s.fromArray(_(b,c.isKanjiModeEnabled()))}})(Uf)),Uf}var Pg;function lx(){if(Pg)return Mf;Pg=1;const s=cs(),e=hd(),t=jy(),i=qy(),o=Yy(),a=Ky(),u=$y(),c=r_(),h=Qy(),d=ex(),m=tx(),_=fs(),g=ax();function S(D,N){const z=D.size,C=a.getPositions(N);for(let w=0;w<C.length;w++){const U=C[w][0],X=C[w][1];for(let k=-1;k<=7;k++)if(!(U+k<=-1||z<=U+k))for(let Y=-1;Y<=7;Y++)X+Y<=-1||z<=X+Y||(k>=0&&k<=6&&(Y===0||Y===6)||Y>=0&&Y<=6&&(k===0||k===6)||k>=2&&k<=4&&Y>=2&&Y<=4?D.set(U+k,X+Y,!0,!0):D.set(U+k,X+Y,!1,!0))}}function M(D){const N=D.size;for(let z=8;z<N-8;z++){const C=z%2===0;D.set(z,6,C,!0),D.set(6,z,C,!0)}}function E(D,N){const z=o.getPositions(N);for(let C=0;C<z.length;C++){const w=z[C][0],U=z[C][1];for(let X=-2;X<=2;X++)for(let k=-2;k<=2;k++)X===-2||X===2||k===-2||k===2||X===0&&k===0?D.set(w+X,U+k,!0,!0):D.set(w+X,U+k,!1,!0)}}function y(D,N){const z=D.size,C=d.getEncodedBits(N);let w,U,X;for(let k=0;k<18;k++)w=Math.floor(k/3),U=k%3+z-8-3,X=(C>>k&1)===1,D.set(w,U,X,!0),D.set(U,w,X,!0)}function x(D,N,z){const C=D.size,w=m.getEncodedBits(N,z);let U,X;for(U=0;U<15;U++)X=(w>>U&1)===1,U<6?D.set(U,8,X,!0):U<8?D.set(U+1,8,X,!0):D.set(C-15+U,8,X,!0),U<8?D.set(8,C-U-1,X,!0):U<9?D.set(8,15-U-1+1,X,!0):D.set(8,15-U-1,X,!0);D.set(C-8,8,1,!0)}function b(D,N){const z=D.size;let C=-1,w=z-1,U=7,X=0;for(let k=z-1;k>0;k-=2)for(k===6&&k--;;){for(let Y=0;Y<2;Y++)if(!D.isReserved(w,k-Y)){let ae=!1;X<N.length&&(ae=(N[X]>>>U&1)===1),D.set(w,k-Y,ae),U--,U===-1&&(X++,U=7)}if(w+=C,w<0||z<=w){w-=C,C=-C;break}}}function T(D,N,z){const C=new t;z.forEach(function(Y){C.put(Y.mode.bit,4),C.put(Y.getLength(),_.getCharCountIndicator(Y.mode,D)),Y.write(C)});const w=s.getSymbolTotalCodewords(D),U=c.getTotalCodewordsCount(D,N),X=(w-U)*8;for(C.getLengthInBits()+4<=X&&C.put(0,4);C.getLengthInBits()%8!==0;)C.putBit(0);const k=(X-C.getLengthInBits())/8;for(let Y=0;Y<k;Y++)C.put(Y%2?17:236,8);return P(C,D,N)}function P(D,N,z){const C=s.getSymbolTotalCodewords(N),w=c.getTotalCodewordsCount(N,z),U=C-w,X=c.getBlocksCount(N,z),k=C%X,Y=X-k,ae=Math.floor(C/X),te=Math.floor(U/X),ce=te+1,G=ae-te,he=new h(G);let re=0;const O=new Array(X),ie=new Array(X);let Ae=0;const Z=new Uint8Array(D.buffer);for(let we=0;we<X;we++){const Ue=we<Y?te:ce;O[we]=Z.slice(re,re+Ue),ie[we]=he.encode(O[we]),re+=Ue,Ae=Math.max(Ae,Ue)}const ne=new Uint8Array(C);let xe=0,de,ee;for(de=0;de<Ae;de++)for(ee=0;ee<X;ee++)de<O[ee].length&&(ne[xe++]=O[ee][de]);for(de=0;de<G;de++)for(ee=0;ee<X;ee++)ne[xe++]=ie[ee][de];return ne}function H(D,N,z,C){let w;if(Array.isArray(D))w=g.fromArray(D);else if(typeof D=="string"){let ae=N;if(!ae){const te=g.rawSplit(D);ae=d.getBestVersionForData(te,z)}w=g.fromString(D,ae||40)}else throw new Error("Invalid data");const U=d.getBestVersionForData(w,z);if(!U)throw new Error("The amount of data is too big to be stored in a QR Code");if(!N)N=U;else if(N<U)throw new Error(`
The chosen QR Code version cannot contain this amount of data.
Minimum version required to store current data is: `+U+`.
`);const X=T(N,z,w),k=s.getSymbolSize(N),Y=new i(k);return S(Y,N),M(Y),E(Y,N),x(Y,z,0),N>=7&&y(Y,N),b(Y,X),isNaN(C)&&(C=u.getBestMask(Y,x.bind(null,Y,z))),u.applyMask(C,Y),x(Y,z,C),{modules:Y,version:N,errorCorrectionLevel:z,maskPattern:C,segments:w}}return Mf.create=function(N,z){if(typeof N>"u"||N==="")throw new Error("No input text");let C=e.M,w,U;return typeof z<"u"&&(C=e.from(z.errorCorrectionLevel,e.M),w=d.from(z.version),U=u.from(z.maskPattern),z.toSJISFunc&&s.setToSJISFunction(z.toSJISFunc)),H(N,w,C,U)},Mf}var Hf={},Vf={},bg;function a_(){return bg||(bg=1,(function(s){function e(t){if(typeof t=="number"&&(t=t.toString()),typeof t!="string")throw new Error("Color should be defined as hex string");let i=t.slice().replace("#","").split("");if(i.length<3||i.length===5||i.length>8)throw new Error("Invalid hex color: "+t);(i.length===3||i.length===4)&&(i=Array.prototype.concat.apply([],i.map(function(a){return[a,a]}))),i.length===6&&i.push("F","F");const o=parseInt(i.join(""),16);return{r:o>>24&255,g:o>>16&255,b:o>>8&255,a:o&255,hex:"#"+i.slice(0,6).join("")}}s.getOptions=function(i){i||(i={}),i.color||(i.color={});const o=typeof i.margin>"u"||i.margin===null||i.margin<0?4:i.margin,a=i.width&&i.width>=21?i.width:void 0,u=i.scale||4;return{width:a,scale:a?4:u,margin:o,color:{dark:e(i.color.dark||"#000000ff"),light:e(i.color.light||"#ffffffff")},type:i.type,rendererOpts:i.rendererOpts||{}}},s.getScale=function(i,o){return o.width&&o.width>=i+o.margin*2?o.width/(i+o.margin*2):o.scale},s.getImageWidth=function(i,o){const a=s.getScale(i,o);return Math.floor((i+o.margin*2)*a)},s.qrToImageData=function(i,o,a){const u=o.modules.size,c=o.modules.data,h=s.getScale(u,a),d=Math.floor((u+a.margin*2)*h),m=a.margin*h,_=[a.color.light,a.color.dark];for(let g=0;g<d;g++)for(let S=0;S<d;S++){let M=(g*d+S)*4,E=a.color.light;if(g>=m&&S>=m&&g<d-m&&S<d-m){const y=Math.floor((g-m)/h),x=Math.floor((S-m)/h);E=_[c[y*u+x]?1:0]}i[M++]=E.r,i[M++]=E.g,i[M++]=E.b,i[M]=E.a}}})(Vf)),Vf}var Lg;function ux(){return Lg||(Lg=1,(function(s){const e=a_();function t(o,a,u){o.clearRect(0,0,a.width,a.height),a.style||(a.style={}),a.height=u,a.width=u,a.style.height=u+"px",a.style.width=u+"px"}function i(){try{return document.createElement("canvas")}catch{throw new Error("You need to specify a canvas element")}}s.render=function(a,u,c){let h=c,d=u;typeof h>"u"&&(!u||!u.getContext)&&(h=u,u=void 0),u||(d=i()),h=e.getOptions(h);const m=e.getImageWidth(a.modules.size,h),_=d.getContext("2d"),g=_.createImageData(m,m);return e.qrToImageData(g.data,a,h),t(_,d,m),_.putImageData(g,0,0),d},s.renderToDataURL=function(a,u,c){let h=c;typeof h>"u"&&(!u||!u.getContext)&&(h=u,u=void 0),h||(h={});const d=s.render(a,u,h),m=h.type||"image/png",_=h.rendererOpts||{};return d.toDataURL(m,_.quality)}})(Hf)),Hf}var Gf={},Dg;function cx(){if(Dg)return Gf;Dg=1;const s=a_();function e(o,a){const u=o.a/255,c=a+'="'+o.hex+'"';return u<1?c+" "+a+'-opacity="'+u.toFixed(2).slice(1)+'"':c}function t(o,a,u){let c=o+a;return typeof u<"u"&&(c+=" "+u),c}function i(o,a,u){let c="",h=0,d=!1,m=0;for(let _=0;_<o.length;_++){const g=Math.floor(_%a),S=Math.floor(_/a);!g&&!d&&(d=!0),o[_]?(m++,_>0&&g>0&&o[_-1]||(c+=d?t("M",g+u,.5+S+u):t("m",h,0),h=0,d=!1),g+1<a&&o[_+1]||(c+=t("h",m),m=0)):h++}return c}return Gf.render=function(a,u,c){const h=s.getOptions(u),d=a.modules.size,m=a.modules.data,_=d+h.margin*2,g=h.color.light.a?"<path "+e(h.color.light,"fill")+' d="M0 0h'+_+"v"+_+'H0z"/>':"",S="<path "+e(h.color.dark,"stroke")+' d="'+i(m,d,h.margin)+'"/>',M='viewBox="0 0 '+_+" "+_+'"',y='<svg xmlns="http://www.w3.org/2000/svg" '+(h.width?'width="'+h.width+'" height="'+h.width+'" ':"")+M+' shape-rendering="crispEdges">'+g+S+`</svg>
`;return typeof c=="function"&&c(null,y),y},Gf}var Ng;function fx(){if(Ng)return Os;Ng=1;const s=Xy(),e=lx(),t=ux(),i=cx();function o(a,u,c,h,d){const m=[].slice.call(arguments,1),_=m.length,g=typeof m[_-1]=="function";if(!g&&!s())throw new Error("Callback required as last argument");if(g){if(_<2)throw new Error("Too few arguments provided");_===2?(d=c,c=u,u=h=void 0):_===3&&(u.getContext&&typeof d>"u"?(d=h,h=void 0):(d=h,h=c,c=u,u=void 0))}else{if(_<1)throw new Error("Too few arguments provided");return _===1?(c=u,u=h=void 0):_===2&&!u.getContext&&(h=c,c=u,u=void 0),new Promise(function(S,M){try{const E=e.create(c,h);S(a(E,u,h))}catch(E){M(E)}})}try{const S=e.create(c,h);d(null,a(S,u,h))}catch(S){d(S)}}return Os.create=e.create,Os.toCanvas=o.bind(null,t.render),Os.toDataURL=o.bind(null,t.renderToDataURL),Os.toString=o.bind(null,function(a,u,c){return i.render(a,c)}),Os}var hx=fx();const Ig=cd(hx);async function Ug(s,e={}){await Ig.toCanvas(s,{errorCorrectionLevel:e.errorCorrection||"M",width:e.size||29,margin:e.margin||1});const t=e.size||29,i=document.createElement("canvas");i.width=t,i.height=t;const o=i.getContext("2d");await new Promise(c=>{Ig.toCanvas(i,s,{errorCorrectionLevel:e.errorCorrection||"M",width:t,margin:0},()=>c())});const a=o.getImageData(0,0,t,t),u=[];for(let c=0;c<t;c++){const h=[];for(let d=0;d<t;d++){const m=(c*t+d)*4;h.push(a.data[m]<128)}u.push(h)}return u}function Wf(s,e,t,i,o,a=0){var m;const u=[],c=s.length,h=((m=s[0])==null?void 0:m.length)||0,d=i*.85;for(let _=0;_<c;_++)for(let g=0;g<h;g++){if(!s[_][g])continue;const S=e+g*i+(i-d)/2,M=t+_*i+(i-d)/2;dx(u,[S,M,a],[d,d,o])}return u}function dx(s,[e,t,i],[o,a,u]){const c=e,h=e+o,d=t,m=t+a,_=i,g=i+u;Bs(s,[c,d,_],[h,d,_],[h,m,_],[c,m,_]),Bs(s,[c,d,g],[h,d,g],[h,m,g],[c,m,g]),Bs(s,[c,d,_],[h,d,_],[h,d,g],[c,d,g]),Bs(s,[h,d,_],[h,m,_],[h,m,g],[h,d,g]),Bs(s,[h,m,_],[c,m,_],[c,m,g],[h,m,g]),Bs(s,[c,m,_],[c,d,_],[c,d,g],[c,m,g])}function Bs(s,e,t,i,o){Fg(s,e,t,i),Fg(s,e,i,o)}function Fg(s,e,t,i){const o=[t[0]-e[0],t[1]-e[1],t[2]-e[2]],a=[i[0]-e[0],i[1]-e[1],i[2]-e[2]],u=[o[1]*a[2]-o[2]*a[1],o[2]*a[0]-o[0]*a[2],o[0]*a[1]-o[1]*a[0]],c=Math.sqrt(u[0]*u[0]+u[1]*u[1]+u[2]*u[2])||1;for(let h=0;h<3;h++)s.push(...Og(u[h]/c));for(const h of[e,t,i])for(let d=0;d<3;d++)s.push(...Og(h[d]));s.push(0,0)}function Og(s){const e=new ArrayBuffer(4);return new DataView(e).setFloat32(0,s,!0),new Uint8Array(e)}function Bg(s){const e=new ArrayBuffer(4);return new DataView(e).setFloat32(0,s,!0),new Uint8Array(e)}function px(s){const e=new ArrayBuffer(2);return new DataView(e).setUint16(0,s,!0),new Uint8Array(e)}function mx(s){const e=Math.sqrt(s[0]*s[0]+s[1]*s[1]+s[2]*s[2])||1;return[s[0]/e,s[1]/e,s[2]/e]}function gx(s,e){return[s[1]*e[2]-s[2]*e[1],s[2]*e[0]-s[0]*e[2],s[0]*e[1]-s[1]*e[0]]}function _x(s,e,t){const i=[e[0]-s[0],e[1]-s[1],e[2]-s[2]],o=[t[0]-s[0],t[1]-s[1],t[2]-s[2]];return mx(gx(i,o))}function zg(s,e,t,i){const o=_x(e,t,i);for(let a=0;a<3;a++)s.push(...Bg(o[a]));for(const a of[e,t,i])for(let u=0;u<3;u++)s.push(...Bg(a[u]));s.push(...px(0))}function vx(s,e,t,i,o){zg(s,e,t,i),zg(s,e,i,o)}function yx(s,e){const[t,i,o]=s,[a,u,c]=e,h=[],d=[[[t,i,o],[t+a,i,o],[t+a,i+u,o],[t,i+u,o]],[[t,i,o+c],[t+a,i,o+c],[t+a,i+u,o+c],[t,i+u,o+c]],[[t,i,o],[t+a,i,o],[t+a,i,o+c],[t,i,o+c]],[[t+a,i,o],[t+a,i+u,o],[t+a,i+u,o+c],[t+a,i,o+c]],[[t+a,i+u,o],[t,i+u,o],[t,i+u,o+c],[t+a,i+u,o+c]],[[t,i+u,o],[t,i,o],[t,i,o+c],[t,i+u,o+c]]];for(const m of d)vx(h,m[0],m[1],m[2],m[3]);return h}function xx(...s){const e=[];for(const t of s)e.push(...t);return e}function Sx(s,e){const t=xx(...e),i=t.length/50,o=new Uint8Array(84),a=new TextEncoder().encode(s.slice(0,79));o.set(a,0);const u=new Uint8Array(84+t.length);return u.set(o,0),new DataView(u.buffer).setUint32(80,i,!0),u.set(t,84),new Blob([u],{type:"application/sla"})}function Mx(s,e){const t=Sx(s,e),i=URL.createObjectURL(t),o=document.createElement("a");o.href=i,o.download=s.replace(/\s+/g,"_")+".stl",o.click(),URL.revokeObjectURL(i)}/**
 * @license
 * Copyright 2010-2024 Three.js Authors
 * SPDX-License-Identifier: MIT
 */const dd="168",ho={ROTATE:0,DOLLY:1,PAN:2},uo={ROTATE:0,PAN:1,DOLLY_PAN:2,DOLLY_ROTATE:3},Ex=0,kg=1,wx=2,l_=1,Tx=2,Wi=3,Dr=0,Bn=1,wi=2,br=0,po=1,Hg=2,Vg=3,Gg=4,Ax=5,ns=100,Cx=101,Rx=102,Px=103,bx=104,Lx=200,Dx=201,Nx=202,Ix=203,Ah=204,Ch=205,Ux=206,Fx=207,Ox=208,Bx=209,zx=210,kx=211,Hx=212,Vx=213,Gx=214,Wx=0,Xx=1,jx=2,Eu=3,qx=4,Yx=5,Kx=6,$x=7,u_=0,Zx=1,Jx=2,Lr=0,Qx=1,eS=2,tS=3,nS=4,iS=5,rS=6,sS=7,c_=300,yo=301,xo=302,Rh=303,Ph=304,Du=306,bh=1e3,rs=1001,Lh=1002,ri=1003,oS=1004,Fl=1005,mn=1006,Xf=1007,ss=1008,Ki=1009,f_=1010,h_=1011,Sa=1012,pd=1013,os=1014,Xi=1015,Aa=1016,md=1017,gd=1018,So=1020,d_=35902,p_=1021,m_=1022,mi=1023,g_=1024,__=1025,mo=1026,Mo=1027,v_=1028,_d=1029,y_=1030,vd=1031,yd=1033,_u=33776,vu=33777,yu=33778,xu=33779,Dh=35840,Nh=35841,Ih=35842,Uh=35843,Fh=36196,Oh=37492,Bh=37496,zh=37808,kh=37809,Hh=37810,Vh=37811,Gh=37812,Wh=37813,Xh=37814,jh=37815,qh=37816,Yh=37817,Kh=37818,$h=37819,Zh=37820,Jh=37821,Su=36492,Qh=36494,ed=36495,x_=36283,td=36284,nd=36285,id=36286,aS=3200,lS=3201,S_=0,uS=1,Rr="",Mi="srgb",Ir="srgb-linear",xd="display-p3",Nu="display-p3-linear",wu="linear",Ft="srgb",Tu="rec709",Au="p3",zs=7680,Wg=519,cS=512,fS=513,hS=514,M_=515,dS=516,pS=517,mS=518,gS=519,rd=35044,Xg="300 es",ji=2e3,Cu=2001;class hs{addEventListener(e,t){this._listeners===void 0&&(this._listeners={});const i=this._listeners;i[e]===void 0&&(i[e]=[]),i[e].indexOf(t)===-1&&i[e].push(t)}hasEventListener(e,t){if(this._listeners===void 0)return!1;const i=this._listeners;return i[e]!==void 0&&i[e].indexOf(t)!==-1}removeEventListener(e,t){if(this._listeners===void 0)return;const o=this._listeners[e];if(o!==void 0){const a=o.indexOf(t);a!==-1&&o.splice(a,1)}}dispatchEvent(e){if(this._listeners===void 0)return;const i=this._listeners[e.type];if(i!==void 0){e.target=this;const o=i.slice(0);for(let a=0,u=o.length;a<u;a++)o[a].call(this,e);e.target=null}}}const Sn=["00","01","02","03","04","05","06","07","08","09","0a","0b","0c","0d","0e","0f","10","11","12","13","14","15","16","17","18","19","1a","1b","1c","1d","1e","1f","20","21","22","23","24","25","26","27","28","29","2a","2b","2c","2d","2e","2f","30","31","32","33","34","35","36","37","38","39","3a","3b","3c","3d","3e","3f","40","41","42","43","44","45","46","47","48","49","4a","4b","4c","4d","4e","4f","50","51","52","53","54","55","56","57","58","59","5a","5b","5c","5d","5e","5f","60","61","62","63","64","65","66","67","68","69","6a","6b","6c","6d","6e","6f","70","71","72","73","74","75","76","77","78","79","7a","7b","7c","7d","7e","7f","80","81","82","83","84","85","86","87","88","89","8a","8b","8c","8d","8e","8f","90","91","92","93","94","95","96","97","98","99","9a","9b","9c","9d","9e","9f","a0","a1","a2","a3","a4","a5","a6","a7","a8","a9","aa","ab","ac","ad","ae","af","b0","b1","b2","b3","b4","b5","b6","b7","b8","b9","ba","bb","bc","bd","be","bf","c0","c1","c2","c3","c4","c5","c6","c7","c8","c9","ca","cb","cc","cd","ce","cf","d0","d1","d2","d3","d4","d5","d6","d7","d8","d9","da","db","dc","dd","de","df","e0","e1","e2","e3","e4","e5","e6","e7","e8","e9","ea","eb","ec","ed","ee","ef","f0","f1","f2","f3","f4","f5","f6","f7","f8","f9","fa","fb","fc","fd","fe","ff"],_a=Math.PI/180,sd=180/Math.PI;function Yi(){const s=Math.random()*4294967295|0,e=Math.random()*4294967295|0,t=Math.random()*4294967295|0,i=Math.random()*4294967295|0;return(Sn[s&255]+Sn[s>>8&255]+Sn[s>>16&255]+Sn[s>>24&255]+"-"+Sn[e&255]+Sn[e>>8&255]+"-"+Sn[e>>16&15|64]+Sn[e>>24&255]+"-"+Sn[t&63|128]+Sn[t>>8&255]+"-"+Sn[t>>16&255]+Sn[t>>24&255]+Sn[i&255]+Sn[i>>8&255]+Sn[i>>16&255]+Sn[i>>24&255]).toLowerCase()}function pn(s,e,t){return Math.max(e,Math.min(t,s))}function _S(s,e){return(s%e+e)%e}function jf(s,e,t){return(1-t)*s+t*e}function Ti(s,e){switch(e.constructor){case Float32Array:return s;case Uint32Array:return s/4294967295;case Uint16Array:return s/65535;case Uint8Array:return s/255;case Int32Array:return Math.max(s/2147483647,-1);case Int16Array:return Math.max(s/32767,-1);case Int8Array:return Math.max(s/127,-1);default:throw new Error("Invalid component type.")}}function Rt(s,e){switch(e.constructor){case Float32Array:return s;case Uint32Array:return Math.round(s*4294967295);case Uint16Array:return Math.round(s*65535);case Uint8Array:return Math.round(s*255);case Int32Array:return Math.round(s*2147483647);case Int16Array:return Math.round(s*32767);case Int8Array:return Math.round(s*127);default:throw new Error("Invalid component type.")}}const vS={DEG2RAD:_a};class Pe{constructor(e=0,t=0){Pe.prototype.isVector2=!0,this.x=e,this.y=t}get width(){return this.x}set width(e){this.x=e}get height(){return this.y}set height(e){this.y=e}set(e,t){return this.x=e,this.y=t,this}setScalar(e){return this.x=e,this.y=e,this}setX(e){return this.x=e,this}setY(e){return this.y=e,this}setComponent(e,t){switch(e){case 0:this.x=t;break;case 1:this.y=t;break;default:throw new Error("index is out of range: "+e)}return this}getComponent(e){switch(e){case 0:return this.x;case 1:return this.y;default:throw new Error("index is out of range: "+e)}}clone(){return new this.constructor(this.x,this.y)}copy(e){return this.x=e.x,this.y=e.y,this}add(e){return this.x+=e.x,this.y+=e.y,this}addScalar(e){return this.x+=e,this.y+=e,this}addVectors(e,t){return this.x=e.x+t.x,this.y=e.y+t.y,this}addScaledVector(e,t){return this.x+=e.x*t,this.y+=e.y*t,this}sub(e){return this.x-=e.x,this.y-=e.y,this}subScalar(e){return this.x-=e,this.y-=e,this}subVectors(e,t){return this.x=e.x-t.x,this.y=e.y-t.y,this}multiply(e){return this.x*=e.x,this.y*=e.y,this}multiplyScalar(e){return this.x*=e,this.y*=e,this}divide(e){return this.x/=e.x,this.y/=e.y,this}divideScalar(e){return this.multiplyScalar(1/e)}applyMatrix3(e){const t=this.x,i=this.y,o=e.elements;return this.x=o[0]*t+o[3]*i+o[6],this.y=o[1]*t+o[4]*i+o[7],this}min(e){return this.x=Math.min(this.x,e.x),this.y=Math.min(this.y,e.y),this}max(e){return this.x=Math.max(this.x,e.x),this.y=Math.max(this.y,e.y),this}clamp(e,t){return this.x=Math.max(e.x,Math.min(t.x,this.x)),this.y=Math.max(e.y,Math.min(t.y,this.y)),this}clampScalar(e,t){return this.x=Math.max(e,Math.min(t,this.x)),this.y=Math.max(e,Math.min(t,this.y)),this}clampLength(e,t){const i=this.length();return this.divideScalar(i||1).multiplyScalar(Math.max(e,Math.min(t,i)))}floor(){return this.x=Math.floor(this.x),this.y=Math.floor(this.y),this}ceil(){return this.x=Math.ceil(this.x),this.y=Math.ceil(this.y),this}round(){return this.x=Math.round(this.x),this.y=Math.round(this.y),this}roundToZero(){return this.x=Math.trunc(this.x),this.y=Math.trunc(this.y),this}negate(){return this.x=-this.x,this.y=-this.y,this}dot(e){return this.x*e.x+this.y*e.y}cross(e){return this.x*e.y-this.y*e.x}lengthSq(){return this.x*this.x+this.y*this.y}length(){return Math.sqrt(this.x*this.x+this.y*this.y)}manhattanLength(){return Math.abs(this.x)+Math.abs(this.y)}normalize(){return this.divideScalar(this.length()||1)}angle(){return Math.atan2(-this.y,-this.x)+Math.PI}angleTo(e){const t=Math.sqrt(this.lengthSq()*e.lengthSq());if(t===0)return Math.PI/2;const i=this.dot(e)/t;return Math.acos(pn(i,-1,1))}distanceTo(e){return Math.sqrt(this.distanceToSquared(e))}distanceToSquared(e){const t=this.x-e.x,i=this.y-e.y;return t*t+i*i}manhattanDistanceTo(e){return Math.abs(this.x-e.x)+Math.abs(this.y-e.y)}setLength(e){return this.normalize().multiplyScalar(e)}lerp(e,t){return this.x+=(e.x-this.x)*t,this.y+=(e.y-this.y)*t,this}lerpVectors(e,t,i){return this.x=e.x+(t.x-e.x)*i,this.y=e.y+(t.y-e.y)*i,this}equals(e){return e.x===this.x&&e.y===this.y}fromArray(e,t=0){return this.x=e[t],this.y=e[t+1],this}toArray(e=[],t=0){return e[t]=this.x,e[t+1]=this.y,e}fromBufferAttribute(e,t){return this.x=e.getX(t),this.y=e.getY(t),this}rotateAround(e,t){const i=Math.cos(t),o=Math.sin(t),a=this.x-e.x,u=this.y-e.y;return this.x=a*i-u*o+e.x,this.y=a*o+u*i+e.y,this}random(){return this.x=Math.random(),this.y=Math.random(),this}*[Symbol.iterator](){yield this.x,yield this.y}}class pt{constructor(e,t,i,o,a,u,c,h,d){pt.prototype.isMatrix3=!0,this.elements=[1,0,0,0,1,0,0,0,1],e!==void 0&&this.set(e,t,i,o,a,u,c,h,d)}set(e,t,i,o,a,u,c,h,d){const m=this.elements;return m[0]=e,m[1]=o,m[2]=c,m[3]=t,m[4]=a,m[5]=h,m[6]=i,m[7]=u,m[8]=d,this}identity(){return this.set(1,0,0,0,1,0,0,0,1),this}copy(e){const t=this.elements,i=e.elements;return t[0]=i[0],t[1]=i[1],t[2]=i[2],t[3]=i[3],t[4]=i[4],t[5]=i[5],t[6]=i[6],t[7]=i[7],t[8]=i[8],this}extractBasis(e,t,i){return e.setFromMatrix3Column(this,0),t.setFromMatrix3Column(this,1),i.setFromMatrix3Column(this,2),this}setFromMatrix4(e){const t=e.elements;return this.set(t[0],t[4],t[8],t[1],t[5],t[9],t[2],t[6],t[10]),this}multiply(e){return this.multiplyMatrices(this,e)}premultiply(e){return this.multiplyMatrices(e,this)}multiplyMatrices(e,t){const i=e.elements,o=t.elements,a=this.elements,u=i[0],c=i[3],h=i[6],d=i[1],m=i[4],_=i[7],g=i[2],S=i[5],M=i[8],E=o[0],y=o[3],x=o[6],b=o[1],T=o[4],P=o[7],H=o[2],D=o[5],N=o[8];return a[0]=u*E+c*b+h*H,a[3]=u*y+c*T+h*D,a[6]=u*x+c*P+h*N,a[1]=d*E+m*b+_*H,a[4]=d*y+m*T+_*D,a[7]=d*x+m*P+_*N,a[2]=g*E+S*b+M*H,a[5]=g*y+S*T+M*D,a[8]=g*x+S*P+M*N,this}multiplyScalar(e){const t=this.elements;return t[0]*=e,t[3]*=e,t[6]*=e,t[1]*=e,t[4]*=e,t[7]*=e,t[2]*=e,t[5]*=e,t[8]*=e,this}determinant(){const e=this.elements,t=e[0],i=e[1],o=e[2],a=e[3],u=e[4],c=e[5],h=e[6],d=e[7],m=e[8];return t*u*m-t*c*d-i*a*m+i*c*h+o*a*d-o*u*h}invert(){const e=this.elements,t=e[0],i=e[1],o=e[2],a=e[3],u=e[4],c=e[5],h=e[6],d=e[7],m=e[8],_=m*u-c*d,g=c*h-m*a,S=d*a-u*h,M=t*_+i*g+o*S;if(M===0)return this.set(0,0,0,0,0,0,0,0,0);const E=1/M;return e[0]=_*E,e[1]=(o*d-m*i)*E,e[2]=(c*i-o*u)*E,e[3]=g*E,e[4]=(m*t-o*h)*E,e[5]=(o*a-c*t)*E,e[6]=S*E,e[7]=(i*h-d*t)*E,e[8]=(u*t-i*a)*E,this}transpose(){let e;const t=this.elements;return e=t[1],t[1]=t[3],t[3]=e,e=t[2],t[2]=t[6],t[6]=e,e=t[5],t[5]=t[7],t[7]=e,this}getNormalMatrix(e){return this.setFromMatrix4(e).invert().transpose()}transposeIntoArray(e){const t=this.elements;return e[0]=t[0],e[1]=t[3],e[2]=t[6],e[3]=t[1],e[4]=t[4],e[5]=t[7],e[6]=t[2],e[7]=t[5],e[8]=t[8],this}setUvTransform(e,t,i,o,a,u,c){const h=Math.cos(a),d=Math.sin(a);return this.set(i*h,i*d,-i*(h*u+d*c)+u+e,-o*d,o*h,-o*(-d*u+h*c)+c+t,0,0,1),this}scale(e,t){return this.premultiply(qf.makeScale(e,t)),this}rotate(e){return this.premultiply(qf.makeRotation(-e)),this}translate(e,t){return this.premultiply(qf.makeTranslation(e,t)),this}makeTranslation(e,t){return e.isVector2?this.set(1,0,e.x,0,1,e.y,0,0,1):this.set(1,0,e,0,1,t,0,0,1),this}makeRotation(e){const t=Math.cos(e),i=Math.sin(e);return this.set(t,-i,0,i,t,0,0,0,1),this}makeScale(e,t){return this.set(e,0,0,0,t,0,0,0,1),this}equals(e){const t=this.elements,i=e.elements;for(let o=0;o<9;o++)if(t[o]!==i[o])return!1;return!0}fromArray(e,t=0){for(let i=0;i<9;i++)this.elements[i]=e[i+t];return this}toArray(e=[],t=0){const i=this.elements;return e[t]=i[0],e[t+1]=i[1],e[t+2]=i[2],e[t+3]=i[3],e[t+4]=i[4],e[t+5]=i[5],e[t+6]=i[6],e[t+7]=i[7],e[t+8]=i[8],e}clone(){return new this.constructor().fromArray(this.elements)}}const qf=new pt;function E_(s){for(let e=s.length-1;e>=0;--e)if(s[e]>=65535)return!0;return!1}function Ru(s){return document.createElementNS("http://www.w3.org/1999/xhtml",s)}function yS(){const s=Ru("canvas");return s.style.display="block",s}const jg={};function go(s){s in jg||(jg[s]=!0,console.warn(s))}function xS(s,e,t){return new Promise(function(i,o){function a(){switch(s.clientWaitSync(e,s.SYNC_FLUSH_COMMANDS_BIT,0)){case s.WAIT_FAILED:o();break;case s.TIMEOUT_EXPIRED:setTimeout(a,t);break;default:i()}}setTimeout(a,t)})}const qg=new pt().set(.8224621,.177538,0,.0331941,.9668058,0,.0170827,.0723974,.9105199),Yg=new pt().set(1.2249401,-.2249404,0,-.0420569,1.0420571,0,-.0196376,-.0786361,1.0982735),oa={[Ir]:{transfer:wu,primaries:Tu,luminanceCoefficients:[.2126,.7152,.0722],toReference:s=>s,fromReference:s=>s},[Mi]:{transfer:Ft,primaries:Tu,luminanceCoefficients:[.2126,.7152,.0722],toReference:s=>s.convertSRGBToLinear(),fromReference:s=>s.convertLinearToSRGB()},[Nu]:{transfer:wu,primaries:Au,luminanceCoefficients:[.2289,.6917,.0793],toReference:s=>s.applyMatrix3(Yg),fromReference:s=>s.applyMatrix3(qg)},[xd]:{transfer:Ft,primaries:Au,luminanceCoefficients:[.2289,.6917,.0793],toReference:s=>s.convertSRGBToLinear().applyMatrix3(Yg),fromReference:s=>s.applyMatrix3(qg).convertLinearToSRGB()}},SS=new Set([Ir,Nu]),Tt={enabled:!0,_workingColorSpace:Ir,get workingColorSpace(){return this._workingColorSpace},set workingColorSpace(s){if(!SS.has(s))throw new Error(`Unsupported working color space, "${s}".`);this._workingColorSpace=s},convert:function(s,e,t){if(this.enabled===!1||e===t||!e||!t)return s;const i=oa[e].toReference,o=oa[t].fromReference;return o(i(s))},fromWorkingColorSpace:function(s,e){return this.convert(s,this._workingColorSpace,e)},toWorkingColorSpace:function(s,e){return this.convert(s,e,this._workingColorSpace)},getPrimaries:function(s){return oa[s].primaries},getTransfer:function(s){return s===Rr?wu:oa[s].transfer},getLuminanceCoefficients:function(s,e=this._workingColorSpace){return s.fromArray(oa[e].luminanceCoefficients)}};function _o(s){return s<.04045?s*.0773993808:Math.pow(s*.9478672986+.0521327014,2.4)}function Yf(s){return s<.0031308?s*12.92:1.055*Math.pow(s,.41666)-.055}let ks;class MS{static getDataURL(e){if(/^data:/i.test(e.src)||typeof HTMLCanvasElement>"u")return e.src;let t;if(e instanceof HTMLCanvasElement)t=e;else{ks===void 0&&(ks=Ru("canvas")),ks.width=e.width,ks.height=e.height;const i=ks.getContext("2d");e instanceof ImageData?i.putImageData(e,0,0):i.drawImage(e,0,0,e.width,e.height),t=ks}return t.width>2048||t.height>2048?(console.warn("THREE.ImageUtils.getDataURL: Image converted to jpg for performance reasons",e),t.toDataURL("image/jpeg",.6)):t.toDataURL("image/png")}static sRGBToLinear(e){if(typeof HTMLImageElement<"u"&&e instanceof HTMLImageElement||typeof HTMLCanvasElement<"u"&&e instanceof HTMLCanvasElement||typeof ImageBitmap<"u"&&e instanceof ImageBitmap){const t=Ru("canvas");t.width=e.width,t.height=e.height;const i=t.getContext("2d");i.drawImage(e,0,0,e.width,e.height);const o=i.getImageData(0,0,e.width,e.height),a=o.data;for(let u=0;u<a.length;u++)a[u]=_o(a[u]/255)*255;return i.putImageData(o,0,0),t}else if(e.data){const t=e.data.slice(0);for(let i=0;i<t.length;i++)t instanceof Uint8Array||t instanceof Uint8ClampedArray?t[i]=Math.floor(_o(t[i]/255)*255):t[i]=_o(t[i]);return{data:t,width:e.width,height:e.height}}else return console.warn("THREE.ImageUtils.sRGBToLinear(): Unsupported image type. No color space conversion applied."),e}}let ES=0;class w_{constructor(e=null){this.isSource=!0,Object.defineProperty(this,"id",{value:ES++}),this.uuid=Yi(),this.data=e,this.dataReady=!0,this.version=0}set needsUpdate(e){e===!0&&this.version++}toJSON(e){const t=e===void 0||typeof e=="string";if(!t&&e.images[this.uuid]!==void 0)return e.images[this.uuid];const i={uuid:this.uuid,url:""},o=this.data;if(o!==null){let a;if(Array.isArray(o)){a=[];for(let u=0,c=o.length;u<c;u++)o[u].isDataTexture?a.push(Kf(o[u].image)):a.push(Kf(o[u]))}else a=Kf(o);i.url=a}return t||(e.images[this.uuid]=i),i}}function Kf(s){return typeof HTMLImageElement<"u"&&s instanceof HTMLImageElement||typeof HTMLCanvasElement<"u"&&s instanceof HTMLCanvasElement||typeof ImageBitmap<"u"&&s instanceof ImageBitmap?MS.getDataURL(s):s.data?{data:Array.from(s.data),width:s.width,height:s.height,type:s.data.constructor.name}:(console.warn("THREE.Texture: Unable to serialize Texture."),{})}let wS=0;class Rn extends hs{constructor(e=Rn.DEFAULT_IMAGE,t=Rn.DEFAULT_MAPPING,i=rs,o=rs,a=mn,u=ss,c=mi,h=Ki,d=Rn.DEFAULT_ANISOTROPY,m=Rr){super(),this.isTexture=!0,Object.defineProperty(this,"id",{value:wS++}),this.uuid=Yi(),this.name="",this.source=new w_(e),this.mipmaps=[],this.mapping=t,this.channel=0,this.wrapS=i,this.wrapT=o,this.magFilter=a,this.minFilter=u,this.anisotropy=d,this.format=c,this.internalFormat=null,this.type=h,this.offset=new Pe(0,0),this.repeat=new Pe(1,1),this.center=new Pe(0,0),this.rotation=0,this.matrixAutoUpdate=!0,this.matrix=new pt,this.generateMipmaps=!0,this.premultiplyAlpha=!1,this.flipY=!0,this.unpackAlignment=4,this.colorSpace=m,this.userData={},this.version=0,this.onUpdate=null,this.isRenderTargetTexture=!1,this.pmremVersion=0}get image(){return this.source.data}set image(e=null){this.source.data=e}updateMatrix(){this.matrix.setUvTransform(this.offset.x,this.offset.y,this.repeat.x,this.repeat.y,this.rotation,this.center.x,this.center.y)}clone(){return new this.constructor().copy(this)}copy(e){return this.name=e.name,this.source=e.source,this.mipmaps=e.mipmaps.slice(0),this.mapping=e.mapping,this.channel=e.channel,this.wrapS=e.wrapS,this.wrapT=e.wrapT,this.magFilter=e.magFilter,this.minFilter=e.minFilter,this.anisotropy=e.anisotropy,this.format=e.format,this.internalFormat=e.internalFormat,this.type=e.type,this.offset.copy(e.offset),this.repeat.copy(e.repeat),this.center.copy(e.center),this.rotation=e.rotation,this.matrixAutoUpdate=e.matrixAutoUpdate,this.matrix.copy(e.matrix),this.generateMipmaps=e.generateMipmaps,this.premultiplyAlpha=e.premultiplyAlpha,this.flipY=e.flipY,this.unpackAlignment=e.unpackAlignment,this.colorSpace=e.colorSpace,this.userData=JSON.parse(JSON.stringify(e.userData)),this.needsUpdate=!0,this}toJSON(e){const t=e===void 0||typeof e=="string";if(!t&&e.textures[this.uuid]!==void 0)return e.textures[this.uuid];const i={metadata:{version:4.6,type:"Texture",generator:"Texture.toJSON"},uuid:this.uuid,name:this.name,image:this.source.toJSON(e).uuid,mapping:this.mapping,channel:this.channel,repeat:[this.repeat.x,this.repeat.y],offset:[this.offset.x,this.offset.y],center:[this.center.x,this.center.y],rotation:this.rotation,wrap:[this.wrapS,this.wrapT],format:this.format,internalFormat:this.internalFormat,type:this.type,colorSpace:this.colorSpace,minFilter:this.minFilter,magFilter:this.magFilter,anisotropy:this.anisotropy,flipY:this.flipY,generateMipmaps:this.generateMipmaps,premultiplyAlpha:this.premultiplyAlpha,unpackAlignment:this.unpackAlignment};return Object.keys(this.userData).length>0&&(i.userData=this.userData),t||(e.textures[this.uuid]=i),i}dispose(){this.dispatchEvent({type:"dispose"})}transformUv(e){if(this.mapping!==c_)return e;if(e.applyMatrix3(this.matrix),e.x<0||e.x>1)switch(this.wrapS){case bh:e.x=e.x-Math.floor(e.x);break;case rs:e.x=e.x<0?0:1;break;case Lh:Math.abs(Math.floor(e.x)%2)===1?e.x=Math.ceil(e.x)-e.x:e.x=e.x-Math.floor(e.x);break}if(e.y<0||e.y>1)switch(this.wrapT){case bh:e.y=e.y-Math.floor(e.y);break;case rs:e.y=e.y<0?0:1;break;case Lh:Math.abs(Math.floor(e.y)%2)===1?e.y=Math.ceil(e.y)-e.y:e.y=e.y-Math.floor(e.y);break}return this.flipY&&(e.y=1-e.y),e}set needsUpdate(e){e===!0&&(this.version++,this.source.needsUpdate=!0)}set needsPMREMUpdate(e){e===!0&&this.pmremVersion++}}Rn.DEFAULT_IMAGE=null;Rn.DEFAULT_MAPPING=c_;Rn.DEFAULT_ANISOTROPY=1;class Qt{constructor(e=0,t=0,i=0,o=1){Qt.prototype.isVector4=!0,this.x=e,this.y=t,this.z=i,this.w=o}get width(){return this.z}set width(e){this.z=e}get height(){return this.w}set height(e){this.w=e}set(e,t,i,o){return this.x=e,this.y=t,this.z=i,this.w=o,this}setScalar(e){return this.x=e,this.y=e,this.z=e,this.w=e,this}setX(e){return this.x=e,this}setY(e){return this.y=e,this}setZ(e){return this.z=e,this}setW(e){return this.w=e,this}setComponent(e,t){switch(e){case 0:this.x=t;break;case 1:this.y=t;break;case 2:this.z=t;break;case 3:this.w=t;break;default:throw new Error("index is out of range: "+e)}return this}getComponent(e){switch(e){case 0:return this.x;case 1:return this.y;case 2:return this.z;case 3:return this.w;default:throw new Error("index is out of range: "+e)}}clone(){return new this.constructor(this.x,this.y,this.z,this.w)}copy(e){return this.x=e.x,this.y=e.y,this.z=e.z,this.w=e.w!==void 0?e.w:1,this}add(e){return this.x+=e.x,this.y+=e.y,this.z+=e.z,this.w+=e.w,this}addScalar(e){return this.x+=e,this.y+=e,this.z+=e,this.w+=e,this}addVectors(e,t){return this.x=e.x+t.x,this.y=e.y+t.y,this.z=e.z+t.z,this.w=e.w+t.w,this}addScaledVector(e,t){return this.x+=e.x*t,this.y+=e.y*t,this.z+=e.z*t,this.w+=e.w*t,this}sub(e){return this.x-=e.x,this.y-=e.y,this.z-=e.z,this.w-=e.w,this}subScalar(e){return this.x-=e,this.y-=e,this.z-=e,this.w-=e,this}subVectors(e,t){return this.x=e.x-t.x,this.y=e.y-t.y,this.z=e.z-t.z,this.w=e.w-t.w,this}multiply(e){return this.x*=e.x,this.y*=e.y,this.z*=e.z,this.w*=e.w,this}multiplyScalar(e){return this.x*=e,this.y*=e,this.z*=e,this.w*=e,this}applyMatrix4(e){const t=this.x,i=this.y,o=this.z,a=this.w,u=e.elements;return this.x=u[0]*t+u[4]*i+u[8]*o+u[12]*a,this.y=u[1]*t+u[5]*i+u[9]*o+u[13]*a,this.z=u[2]*t+u[6]*i+u[10]*o+u[14]*a,this.w=u[3]*t+u[7]*i+u[11]*o+u[15]*a,this}divideScalar(e){return this.multiplyScalar(1/e)}setAxisAngleFromQuaternion(e){this.w=2*Math.acos(e.w);const t=Math.sqrt(1-e.w*e.w);return t<1e-4?(this.x=1,this.y=0,this.z=0):(this.x=e.x/t,this.y=e.y/t,this.z=e.z/t),this}setAxisAngleFromRotationMatrix(e){let t,i,o,a;const h=e.elements,d=h[0],m=h[4],_=h[8],g=h[1],S=h[5],M=h[9],E=h[2],y=h[6],x=h[10];if(Math.abs(m-g)<.01&&Math.abs(_-E)<.01&&Math.abs(M-y)<.01){if(Math.abs(m+g)<.1&&Math.abs(_+E)<.1&&Math.abs(M+y)<.1&&Math.abs(d+S+x-3)<.1)return this.set(1,0,0,0),this;t=Math.PI;const T=(d+1)/2,P=(S+1)/2,H=(x+1)/2,D=(m+g)/4,N=(_+E)/4,z=(M+y)/4;return T>P&&T>H?T<.01?(i=0,o=.707106781,a=.707106781):(i=Math.sqrt(T),o=D/i,a=N/i):P>H?P<.01?(i=.707106781,o=0,a=.707106781):(o=Math.sqrt(P),i=D/o,a=z/o):H<.01?(i=.707106781,o=.707106781,a=0):(a=Math.sqrt(H),i=N/a,o=z/a),this.set(i,o,a,t),this}let b=Math.sqrt((y-M)*(y-M)+(_-E)*(_-E)+(g-m)*(g-m));return Math.abs(b)<.001&&(b=1),this.x=(y-M)/b,this.y=(_-E)/b,this.z=(g-m)/b,this.w=Math.acos((d+S+x-1)/2),this}setFromMatrixPosition(e){const t=e.elements;return this.x=t[12],this.y=t[13],this.z=t[14],this.w=t[15],this}min(e){return this.x=Math.min(this.x,e.x),this.y=Math.min(this.y,e.y),this.z=Math.min(this.z,e.z),this.w=Math.min(this.w,e.w),this}max(e){return this.x=Math.max(this.x,e.x),this.y=Math.max(this.y,e.y),this.z=Math.max(this.z,e.z),this.w=Math.max(this.w,e.w),this}clamp(e,t){return this.x=Math.max(e.x,Math.min(t.x,this.x)),this.y=Math.max(e.y,Math.min(t.y,this.y)),this.z=Math.max(e.z,Math.min(t.z,this.z)),this.w=Math.max(e.w,Math.min(t.w,this.w)),this}clampScalar(e,t){return this.x=Math.max(e,Math.min(t,this.x)),this.y=Math.max(e,Math.min(t,this.y)),this.z=Math.max(e,Math.min(t,this.z)),this.w=Math.max(e,Math.min(t,this.w)),this}clampLength(e,t){const i=this.length();return this.divideScalar(i||1).multiplyScalar(Math.max(e,Math.min(t,i)))}floor(){return this.x=Math.floor(this.x),this.y=Math.floor(this.y),this.z=Math.floor(this.z),this.w=Math.floor(this.w),this}ceil(){return this.x=Math.ceil(this.x),this.y=Math.ceil(this.y),this.z=Math.ceil(this.z),this.w=Math.ceil(this.w),this}round(){return this.x=Math.round(this.x),this.y=Math.round(this.y),this.z=Math.round(this.z),this.w=Math.round(this.w),this}roundToZero(){return this.x=Math.trunc(this.x),this.y=Math.trunc(this.y),this.z=Math.trunc(this.z),this.w=Math.trunc(this.w),this}negate(){return this.x=-this.x,this.y=-this.y,this.z=-this.z,this.w=-this.w,this}dot(e){return this.x*e.x+this.y*e.y+this.z*e.z+this.w*e.w}lengthSq(){return this.x*this.x+this.y*this.y+this.z*this.z+this.w*this.w}length(){return Math.sqrt(this.x*this.x+this.y*this.y+this.z*this.z+this.w*this.w)}manhattanLength(){return Math.abs(this.x)+Math.abs(this.y)+Math.abs(this.z)+Math.abs(this.w)}normalize(){return this.divideScalar(this.length()||1)}setLength(e){return this.normalize().multiplyScalar(e)}lerp(e,t){return this.x+=(e.x-this.x)*t,this.y+=(e.y-this.y)*t,this.z+=(e.z-this.z)*t,this.w+=(e.w-this.w)*t,this}lerpVectors(e,t,i){return this.x=e.x+(t.x-e.x)*i,this.y=e.y+(t.y-e.y)*i,this.z=e.z+(t.z-e.z)*i,this.w=e.w+(t.w-e.w)*i,this}equals(e){return e.x===this.x&&e.y===this.y&&e.z===this.z&&e.w===this.w}fromArray(e,t=0){return this.x=e[t],this.y=e[t+1],this.z=e[t+2],this.w=e[t+3],this}toArray(e=[],t=0){return e[t]=this.x,e[t+1]=this.y,e[t+2]=this.z,e[t+3]=this.w,e}fromBufferAttribute(e,t){return this.x=e.getX(t),this.y=e.getY(t),this.z=e.getZ(t),this.w=e.getW(t),this}random(){return this.x=Math.random(),this.y=Math.random(),this.z=Math.random(),this.w=Math.random(),this}*[Symbol.iterator](){yield this.x,yield this.y,yield this.z,yield this.w}}class TS extends hs{constructor(e=1,t=1,i={}){super(),this.isRenderTarget=!0,this.width=e,this.height=t,this.depth=1,this.scissor=new Qt(0,0,e,t),this.scissorTest=!1,this.viewport=new Qt(0,0,e,t);const o={width:e,height:t,depth:1};i=Object.assign({generateMipmaps:!1,internalFormat:null,minFilter:mn,depthBuffer:!0,stencilBuffer:!1,resolveDepthBuffer:!0,resolveStencilBuffer:!0,depthTexture:null,samples:0,count:1},i);const a=new Rn(o,i.mapping,i.wrapS,i.wrapT,i.magFilter,i.minFilter,i.format,i.type,i.anisotropy,i.colorSpace);a.flipY=!1,a.generateMipmaps=i.generateMipmaps,a.internalFormat=i.internalFormat,this.textures=[];const u=i.count;for(let c=0;c<u;c++)this.textures[c]=a.clone(),this.textures[c].isRenderTargetTexture=!0;this.depthBuffer=i.depthBuffer,this.stencilBuffer=i.stencilBuffer,this.resolveDepthBuffer=i.resolveDepthBuffer,this.resolveStencilBuffer=i.resolveStencilBuffer,this.depthTexture=i.depthTexture,this.samples=i.samples}get texture(){return this.textures[0]}set texture(e){this.textures[0]=e}setSize(e,t,i=1){if(this.width!==e||this.height!==t||this.depth!==i){this.width=e,this.height=t,this.depth=i;for(let o=0,a=this.textures.length;o<a;o++)this.textures[o].image.width=e,this.textures[o].image.height=t,this.textures[o].image.depth=i;this.dispose()}this.viewport.set(0,0,e,t),this.scissor.set(0,0,e,t)}clone(){return new this.constructor().copy(this)}copy(e){this.width=e.width,this.height=e.height,this.depth=e.depth,this.scissor.copy(e.scissor),this.scissorTest=e.scissorTest,this.viewport.copy(e.viewport),this.textures.length=0;for(let i=0,o=e.textures.length;i<o;i++)this.textures[i]=e.textures[i].clone(),this.textures[i].isRenderTargetTexture=!0;const t=Object.assign({},e.texture.image);return this.texture.source=new w_(t),this.depthBuffer=e.depthBuffer,this.stencilBuffer=e.stencilBuffer,this.resolveDepthBuffer=e.resolveDepthBuffer,this.resolveStencilBuffer=e.resolveStencilBuffer,e.depthTexture!==null&&(this.depthTexture=e.depthTexture.clone()),this.samples=e.samples,this}dispose(){this.dispatchEvent({type:"dispose"})}}class as extends TS{constructor(e=1,t=1,i={}){super(e,t,i),this.isWebGLRenderTarget=!0}}class T_ extends Rn{constructor(e=null,t=1,i=1,o=1){super(null),this.isDataArrayTexture=!0,this.image={data:e,width:t,height:i,depth:o},this.magFilter=ri,this.minFilter=ri,this.wrapR=rs,this.generateMipmaps=!1,this.flipY=!1,this.unpackAlignment=1,this.layerUpdates=new Set}addLayerUpdate(e){this.layerUpdates.add(e)}clearLayerUpdates(){this.layerUpdates.clear()}}class AS extends Rn{constructor(e=null,t=1,i=1,o=1){super(null),this.isData3DTexture=!0,this.image={data:e,width:t,height:i,depth:o},this.magFilter=ri,this.minFilter=ri,this.wrapR=rs,this.generateMipmaps=!1,this.flipY=!1,this.unpackAlignment=1}}class ls{constructor(e=0,t=0,i=0,o=1){this.isQuaternion=!0,this._x=e,this._y=t,this._z=i,this._w=o}static slerpFlat(e,t,i,o,a,u,c){let h=i[o+0],d=i[o+1],m=i[o+2],_=i[o+3];const g=a[u+0],S=a[u+1],M=a[u+2],E=a[u+3];if(c===0){e[t+0]=h,e[t+1]=d,e[t+2]=m,e[t+3]=_;return}if(c===1){e[t+0]=g,e[t+1]=S,e[t+2]=M,e[t+3]=E;return}if(_!==E||h!==g||d!==S||m!==M){let y=1-c;const x=h*g+d*S+m*M+_*E,b=x>=0?1:-1,T=1-x*x;if(T>Number.EPSILON){const H=Math.sqrt(T),D=Math.atan2(H,x*b);y=Math.sin(y*D)/H,c=Math.sin(c*D)/H}const P=c*b;if(h=h*y+g*P,d=d*y+S*P,m=m*y+M*P,_=_*y+E*P,y===1-c){const H=1/Math.sqrt(h*h+d*d+m*m+_*_);h*=H,d*=H,m*=H,_*=H}}e[t]=h,e[t+1]=d,e[t+2]=m,e[t+3]=_}static multiplyQuaternionsFlat(e,t,i,o,a,u){const c=i[o],h=i[o+1],d=i[o+2],m=i[o+3],_=a[u],g=a[u+1],S=a[u+2],M=a[u+3];return e[t]=c*M+m*_+h*S-d*g,e[t+1]=h*M+m*g+d*_-c*S,e[t+2]=d*M+m*S+c*g-h*_,e[t+3]=m*M-c*_-h*g-d*S,e}get x(){return this._x}set x(e){this._x=e,this._onChangeCallback()}get y(){return this._y}set y(e){this._y=e,this._onChangeCallback()}get z(){return this._z}set z(e){this._z=e,this._onChangeCallback()}get w(){return this._w}set w(e){this._w=e,this._onChangeCallback()}set(e,t,i,o){return this._x=e,this._y=t,this._z=i,this._w=o,this._onChangeCallback(),this}clone(){return new this.constructor(this._x,this._y,this._z,this._w)}copy(e){return this._x=e.x,this._y=e.y,this._z=e.z,this._w=e.w,this._onChangeCallback(),this}setFromEuler(e,t=!0){const i=e._x,o=e._y,a=e._z,u=e._order,c=Math.cos,h=Math.sin,d=c(i/2),m=c(o/2),_=c(a/2),g=h(i/2),S=h(o/2),M=h(a/2);switch(u){case"XYZ":this._x=g*m*_+d*S*M,this._y=d*S*_-g*m*M,this._z=d*m*M+g*S*_,this._w=d*m*_-g*S*M;break;case"YXZ":this._x=g*m*_+d*S*M,this._y=d*S*_-g*m*M,this._z=d*m*M-g*S*_,this._w=d*m*_+g*S*M;break;case"ZXY":this._x=g*m*_-d*S*M,this._y=d*S*_+g*m*M,this._z=d*m*M+g*S*_,this._w=d*m*_-g*S*M;break;case"ZYX":this._x=g*m*_-d*S*M,this._y=d*S*_+g*m*M,this._z=d*m*M-g*S*_,this._w=d*m*_+g*S*M;break;case"YZX":this._x=g*m*_+d*S*M,this._y=d*S*_+g*m*M,this._z=d*m*M-g*S*_,this._w=d*m*_-g*S*M;break;case"XZY":this._x=g*m*_-d*S*M,this._y=d*S*_-g*m*M,this._z=d*m*M+g*S*_,this._w=d*m*_+g*S*M;break;default:console.warn("THREE.Quaternion: .setFromEuler() encountered an unknown order: "+u)}return t===!0&&this._onChangeCallback(),this}setFromAxisAngle(e,t){const i=t/2,o=Math.sin(i);return this._x=e.x*o,this._y=e.y*o,this._z=e.z*o,this._w=Math.cos(i),this._onChangeCallback(),this}setFromRotationMatrix(e){const t=e.elements,i=t[0],o=t[4],a=t[8],u=t[1],c=t[5],h=t[9],d=t[2],m=t[6],_=t[10],g=i+c+_;if(g>0){const S=.5/Math.sqrt(g+1);this._w=.25/S,this._x=(m-h)*S,this._y=(a-d)*S,this._z=(u-o)*S}else if(i>c&&i>_){const S=2*Math.sqrt(1+i-c-_);this._w=(m-h)/S,this._x=.25*S,this._y=(o+u)/S,this._z=(a+d)/S}else if(c>_){const S=2*Math.sqrt(1+c-i-_);this._w=(a-d)/S,this._x=(o+u)/S,this._y=.25*S,this._z=(h+m)/S}else{const S=2*Math.sqrt(1+_-i-c);this._w=(u-o)/S,this._x=(a+d)/S,this._y=(h+m)/S,this._z=.25*S}return this._onChangeCallback(),this}setFromUnitVectors(e,t){let i=e.dot(t)+1;return i<Number.EPSILON?(i=0,Math.abs(e.x)>Math.abs(e.z)?(this._x=-e.y,this._y=e.x,this._z=0,this._w=i):(this._x=0,this._y=-e.z,this._z=e.y,this._w=i)):(this._x=e.y*t.z-e.z*t.y,this._y=e.z*t.x-e.x*t.z,this._z=e.x*t.y-e.y*t.x,this._w=i),this.normalize()}angleTo(e){return 2*Math.acos(Math.abs(pn(this.dot(e),-1,1)))}rotateTowards(e,t){const i=this.angleTo(e);if(i===0)return this;const o=Math.min(1,t/i);return this.slerp(e,o),this}identity(){return this.set(0,0,0,1)}invert(){return this.conjugate()}conjugate(){return this._x*=-1,this._y*=-1,this._z*=-1,this._onChangeCallback(),this}dot(e){return this._x*e._x+this._y*e._y+this._z*e._z+this._w*e._w}lengthSq(){return this._x*this._x+this._y*this._y+this._z*this._z+this._w*this._w}length(){return Math.sqrt(this._x*this._x+this._y*this._y+this._z*this._z+this._w*this._w)}normalize(){let e=this.length();return e===0?(this._x=0,this._y=0,this._z=0,this._w=1):(e=1/e,this._x=this._x*e,this._y=this._y*e,this._z=this._z*e,this._w=this._w*e),this._onChangeCallback(),this}multiply(e){return this.multiplyQuaternions(this,e)}premultiply(e){return this.multiplyQuaternions(e,this)}multiplyQuaternions(e,t){const i=e._x,o=e._y,a=e._z,u=e._w,c=t._x,h=t._y,d=t._z,m=t._w;return this._x=i*m+u*c+o*d-a*h,this._y=o*m+u*h+a*c-i*d,this._z=a*m+u*d+i*h-o*c,this._w=u*m-i*c-o*h-a*d,this._onChangeCallback(),this}slerp(e,t){if(t===0)return this;if(t===1)return this.copy(e);const i=this._x,o=this._y,a=this._z,u=this._w;let c=u*e._w+i*e._x+o*e._y+a*e._z;if(c<0?(this._w=-e._w,this._x=-e._x,this._y=-e._y,this._z=-e._z,c=-c):this.copy(e),c>=1)return this._w=u,this._x=i,this._y=o,this._z=a,this;const h=1-c*c;if(h<=Number.EPSILON){const S=1-t;return this._w=S*u+t*this._w,this._x=S*i+t*this._x,this._y=S*o+t*this._y,this._z=S*a+t*this._z,this.normalize(),this}const d=Math.sqrt(h),m=Math.atan2(d,c),_=Math.sin((1-t)*m)/d,g=Math.sin(t*m)/d;return this._w=u*_+this._w*g,this._x=i*_+this._x*g,this._y=o*_+this._y*g,this._z=a*_+this._z*g,this._onChangeCallback(),this}slerpQuaternions(e,t,i){return this.copy(e).slerp(t,i)}random(){const e=2*Math.PI*Math.random(),t=2*Math.PI*Math.random(),i=Math.random(),o=Math.sqrt(1-i),a=Math.sqrt(i);return this.set(o*Math.sin(e),o*Math.cos(e),a*Math.sin(t),a*Math.cos(t))}equals(e){return e._x===this._x&&e._y===this._y&&e._z===this._z&&e._w===this._w}fromArray(e,t=0){return this._x=e[t],this._y=e[t+1],this._z=e[t+2],this._w=e[t+3],this._onChangeCallback(),this}toArray(e=[],t=0){return e[t]=this._x,e[t+1]=this._y,e[t+2]=this._z,e[t+3]=this._w,e}fromBufferAttribute(e,t){return this._x=e.getX(t),this._y=e.getY(t),this._z=e.getZ(t),this._w=e.getW(t),this._onChangeCallback(),this}toJSON(){return this.toArray()}_onChange(e){return this._onChangeCallback=e,this}_onChangeCallback(){}*[Symbol.iterator](){yield this._x,yield this._y,yield this._z,yield this._w}}class W{constructor(e=0,t=0,i=0){W.prototype.isVector3=!0,this.x=e,this.y=t,this.z=i}set(e,t,i){return i===void 0&&(i=this.z),this.x=e,this.y=t,this.z=i,this}setScalar(e){return this.x=e,this.y=e,this.z=e,this}setX(e){return this.x=e,this}setY(e){return this.y=e,this}setZ(e){return this.z=e,this}setComponent(e,t){switch(e){case 0:this.x=t;break;case 1:this.y=t;break;case 2:this.z=t;break;default:throw new Error("index is out of range: "+e)}return this}getComponent(e){switch(e){case 0:return this.x;case 1:return this.y;case 2:return this.z;default:throw new Error("index is out of range: "+e)}}clone(){return new this.constructor(this.x,this.y,this.z)}copy(e){return this.x=e.x,this.y=e.y,this.z=e.z,this}add(e){return this.x+=e.x,this.y+=e.y,this.z+=e.z,this}addScalar(e){return this.x+=e,this.y+=e,this.z+=e,this}addVectors(e,t){return this.x=e.x+t.x,this.y=e.y+t.y,this.z=e.z+t.z,this}addScaledVector(e,t){return this.x+=e.x*t,this.y+=e.y*t,this.z+=e.z*t,this}sub(e){return this.x-=e.x,this.y-=e.y,this.z-=e.z,this}subScalar(e){return this.x-=e,this.y-=e,this.z-=e,this}subVectors(e,t){return this.x=e.x-t.x,this.y=e.y-t.y,this.z=e.z-t.z,this}multiply(e){return this.x*=e.x,this.y*=e.y,this.z*=e.z,this}multiplyScalar(e){return this.x*=e,this.y*=e,this.z*=e,this}multiplyVectors(e,t){return this.x=e.x*t.x,this.y=e.y*t.y,this.z=e.z*t.z,this}applyEuler(e){return this.applyQuaternion(Kg.setFromEuler(e))}applyAxisAngle(e,t){return this.applyQuaternion(Kg.setFromAxisAngle(e,t))}applyMatrix3(e){const t=this.x,i=this.y,o=this.z,a=e.elements;return this.x=a[0]*t+a[3]*i+a[6]*o,this.y=a[1]*t+a[4]*i+a[7]*o,this.z=a[2]*t+a[5]*i+a[8]*o,this}applyNormalMatrix(e){return this.applyMatrix3(e).normalize()}applyMatrix4(e){const t=this.x,i=this.y,o=this.z,a=e.elements,u=1/(a[3]*t+a[7]*i+a[11]*o+a[15]);return this.x=(a[0]*t+a[4]*i+a[8]*o+a[12])*u,this.y=(a[1]*t+a[5]*i+a[9]*o+a[13])*u,this.z=(a[2]*t+a[6]*i+a[10]*o+a[14])*u,this}applyQuaternion(e){const t=this.x,i=this.y,o=this.z,a=e.x,u=e.y,c=e.z,h=e.w,d=2*(u*o-c*i),m=2*(c*t-a*o),_=2*(a*i-u*t);return this.x=t+h*d+u*_-c*m,this.y=i+h*m+c*d-a*_,this.z=o+h*_+a*m-u*d,this}project(e){return this.applyMatrix4(e.matrixWorldInverse).applyMatrix4(e.projectionMatrix)}unproject(e){return this.applyMatrix4(e.projectionMatrixInverse).applyMatrix4(e.matrixWorld)}transformDirection(e){const t=this.x,i=this.y,o=this.z,a=e.elements;return this.x=a[0]*t+a[4]*i+a[8]*o,this.y=a[1]*t+a[5]*i+a[9]*o,this.z=a[2]*t+a[6]*i+a[10]*o,this.normalize()}divide(e){return this.x/=e.x,this.y/=e.y,this.z/=e.z,this}divideScalar(e){return this.multiplyScalar(1/e)}min(e){return this.x=Math.min(this.x,e.x),this.y=Math.min(this.y,e.y),this.z=Math.min(this.z,e.z),this}max(e){return this.x=Math.max(this.x,e.x),this.y=Math.max(this.y,e.y),this.z=Math.max(this.z,e.z),this}clamp(e,t){return this.x=Math.max(e.x,Math.min(t.x,this.x)),this.y=Math.max(e.y,Math.min(t.y,this.y)),this.z=Math.max(e.z,Math.min(t.z,this.z)),this}clampScalar(e,t){return this.x=Math.max(e,Math.min(t,this.x)),this.y=Math.max(e,Math.min(t,this.y)),this.z=Math.max(e,Math.min(t,this.z)),this}clampLength(e,t){const i=this.length();return this.divideScalar(i||1).multiplyScalar(Math.max(e,Math.min(t,i)))}floor(){return this.x=Math.floor(this.x),this.y=Math.floor(this.y),this.z=Math.floor(this.z),this}ceil(){return this.x=Math.ceil(this.x),this.y=Math.ceil(this.y),this.z=Math.ceil(this.z),this}round(){return this.x=Math.round(this.x),this.y=Math.round(this.y),this.z=Math.round(this.z),this}roundToZero(){return this.x=Math.trunc(this.x),this.y=Math.trunc(this.y),this.z=Math.trunc(this.z),this}negate(){return this.x=-this.x,this.y=-this.y,this.z=-this.z,this}dot(e){return this.x*e.x+this.y*e.y+this.z*e.z}lengthSq(){return this.x*this.x+this.y*this.y+this.z*this.z}length(){return Math.sqrt(this.x*this.x+this.y*this.y+this.z*this.z)}manhattanLength(){return Math.abs(this.x)+Math.abs(this.y)+Math.abs(this.z)}normalize(){return this.divideScalar(this.length()||1)}setLength(e){return this.normalize().multiplyScalar(e)}lerp(e,t){return this.x+=(e.x-this.x)*t,this.y+=(e.y-this.y)*t,this.z+=(e.z-this.z)*t,this}lerpVectors(e,t,i){return this.x=e.x+(t.x-e.x)*i,this.y=e.y+(t.y-e.y)*i,this.z=e.z+(t.z-e.z)*i,this}cross(e){return this.crossVectors(this,e)}crossVectors(e,t){const i=e.x,o=e.y,a=e.z,u=t.x,c=t.y,h=t.z;return this.x=o*h-a*c,this.y=a*u-i*h,this.z=i*c-o*u,this}projectOnVector(e){const t=e.lengthSq();if(t===0)return this.set(0,0,0);const i=e.dot(this)/t;return this.copy(e).multiplyScalar(i)}projectOnPlane(e){return $f.copy(this).projectOnVector(e),this.sub($f)}reflect(e){return this.sub($f.copy(e).multiplyScalar(2*this.dot(e)))}angleTo(e){const t=Math.sqrt(this.lengthSq()*e.lengthSq());if(t===0)return Math.PI/2;const i=this.dot(e)/t;return Math.acos(pn(i,-1,1))}distanceTo(e){return Math.sqrt(this.distanceToSquared(e))}distanceToSquared(e){const t=this.x-e.x,i=this.y-e.y,o=this.z-e.z;return t*t+i*i+o*o}manhattanDistanceTo(e){return Math.abs(this.x-e.x)+Math.abs(this.y-e.y)+Math.abs(this.z-e.z)}setFromSpherical(e){return this.setFromSphericalCoords(e.radius,e.phi,e.theta)}setFromSphericalCoords(e,t,i){const o=Math.sin(t)*e;return this.x=o*Math.sin(i),this.y=Math.cos(t)*e,this.z=o*Math.cos(i),this}setFromCylindrical(e){return this.setFromCylindricalCoords(e.radius,e.theta,e.y)}setFromCylindricalCoords(e,t,i){return this.x=e*Math.sin(t),this.y=i,this.z=e*Math.cos(t),this}setFromMatrixPosition(e){const t=e.elements;return this.x=t[12],this.y=t[13],this.z=t[14],this}setFromMatrixScale(e){const t=this.setFromMatrixColumn(e,0).length(),i=this.setFromMatrixColumn(e,1).length(),o=this.setFromMatrixColumn(e,2).length();return this.x=t,this.y=i,this.z=o,this}setFromMatrixColumn(e,t){return this.fromArray(e.elements,t*4)}setFromMatrix3Column(e,t){return this.fromArray(e.elements,t*3)}setFromEuler(e){return this.x=e._x,this.y=e._y,this.z=e._z,this}setFromColor(e){return this.x=e.r,this.y=e.g,this.z=e.b,this}equals(e){return e.x===this.x&&e.y===this.y&&e.z===this.z}fromArray(e,t=0){return this.x=e[t],this.y=e[t+1],this.z=e[t+2],this}toArray(e=[],t=0){return e[t]=this.x,e[t+1]=this.y,e[t+2]=this.z,e}fromBufferAttribute(e,t){return this.x=e.getX(t),this.y=e.getY(t),this.z=e.getZ(t),this}random(){return this.x=Math.random(),this.y=Math.random(),this.z=Math.random(),this}randomDirection(){const e=Math.random()*Math.PI*2,t=Math.random()*2-1,i=Math.sqrt(1-t*t);return this.x=i*Math.cos(e),this.y=t,this.z=i*Math.sin(e),this}*[Symbol.iterator](){yield this.x,yield this.y,yield this.z}}const $f=new W,Kg=new ls;class Ca{constructor(e=new W(1/0,1/0,1/0),t=new W(-1/0,-1/0,-1/0)){this.isBox3=!0,this.min=e,this.max=t}set(e,t){return this.min.copy(e),this.max.copy(t),this}setFromArray(e){this.makeEmpty();for(let t=0,i=e.length;t<i;t+=3)this.expandByPoint(hi.fromArray(e,t));return this}setFromBufferAttribute(e){this.makeEmpty();for(let t=0,i=e.count;t<i;t++)this.expandByPoint(hi.fromBufferAttribute(e,t));return this}setFromPoints(e){this.makeEmpty();for(let t=0,i=e.length;t<i;t++)this.expandByPoint(e[t]);return this}setFromCenterAndSize(e,t){const i=hi.copy(t).multiplyScalar(.5);return this.min.copy(e).sub(i),this.max.copy(e).add(i),this}setFromObject(e,t=!1){return this.makeEmpty(),this.expandByObject(e,t)}clone(){return new this.constructor().copy(this)}copy(e){return this.min.copy(e.min),this.max.copy(e.max),this}makeEmpty(){return this.min.x=this.min.y=this.min.z=1/0,this.max.x=this.max.y=this.max.z=-1/0,this}isEmpty(){return this.max.x<this.min.x||this.max.y<this.min.y||this.max.z<this.min.z}getCenter(e){return this.isEmpty()?e.set(0,0,0):e.addVectors(this.min,this.max).multiplyScalar(.5)}getSize(e){return this.isEmpty()?e.set(0,0,0):e.subVectors(this.max,this.min)}expandByPoint(e){return this.min.min(e),this.max.max(e),this}expandByVector(e){return this.min.sub(e),this.max.add(e),this}expandByScalar(e){return this.min.addScalar(-e),this.max.addScalar(e),this}expandByObject(e,t=!1){e.updateWorldMatrix(!1,!1);const i=e.geometry;if(i!==void 0){const a=i.getAttribute("position");if(t===!0&&a!==void 0&&e.isInstancedMesh!==!0)for(let u=0,c=a.count;u<c;u++)e.isMesh===!0?e.getVertexPosition(u,hi):hi.fromBufferAttribute(a,u),hi.applyMatrix4(e.matrixWorld),this.expandByPoint(hi);else e.boundingBox!==void 0?(e.boundingBox===null&&e.computeBoundingBox(),Ol.copy(e.boundingBox)):(i.boundingBox===null&&i.computeBoundingBox(),Ol.copy(i.boundingBox)),Ol.applyMatrix4(e.matrixWorld),this.union(Ol)}const o=e.children;for(let a=0,u=o.length;a<u;a++)this.expandByObject(o[a],t);return this}containsPoint(e){return e.x>=this.min.x&&e.x<=this.max.x&&e.y>=this.min.y&&e.y<=this.max.y&&e.z>=this.min.z&&e.z<=this.max.z}containsBox(e){return this.min.x<=e.min.x&&e.max.x<=this.max.x&&this.min.y<=e.min.y&&e.max.y<=this.max.y&&this.min.z<=e.min.z&&e.max.z<=this.max.z}getParameter(e,t){return t.set((e.x-this.min.x)/(this.max.x-this.min.x),(e.y-this.min.y)/(this.max.y-this.min.y),(e.z-this.min.z)/(this.max.z-this.min.z))}intersectsBox(e){return e.max.x>=this.min.x&&e.min.x<=this.max.x&&e.max.y>=this.min.y&&e.min.y<=this.max.y&&e.max.z>=this.min.z&&e.min.z<=this.max.z}intersectsSphere(e){return this.clampPoint(e.center,hi),hi.distanceToSquared(e.center)<=e.radius*e.radius}intersectsPlane(e){let t,i;return e.normal.x>0?(t=e.normal.x*this.min.x,i=e.normal.x*this.max.x):(t=e.normal.x*this.max.x,i=e.normal.x*this.min.x),e.normal.y>0?(t+=e.normal.y*this.min.y,i+=e.normal.y*this.max.y):(t+=e.normal.y*this.max.y,i+=e.normal.y*this.min.y),e.normal.z>0?(t+=e.normal.z*this.min.z,i+=e.normal.z*this.max.z):(t+=e.normal.z*this.max.z,i+=e.normal.z*this.min.z),t<=-e.constant&&i>=-e.constant}intersectsTriangle(e){if(this.isEmpty())return!1;this.getCenter(aa),Bl.subVectors(this.max,aa),Hs.subVectors(e.a,aa),Vs.subVectors(e.b,aa),Gs.subVectors(e.c,aa),Sr.subVectors(Vs,Hs),Mr.subVectors(Gs,Vs),Kr.subVectors(Hs,Gs);let t=[0,-Sr.z,Sr.y,0,-Mr.z,Mr.y,0,-Kr.z,Kr.y,Sr.z,0,-Sr.x,Mr.z,0,-Mr.x,Kr.z,0,-Kr.x,-Sr.y,Sr.x,0,-Mr.y,Mr.x,0,-Kr.y,Kr.x,0];return!Zf(t,Hs,Vs,Gs,Bl)||(t=[1,0,0,0,1,0,0,0,1],!Zf(t,Hs,Vs,Gs,Bl))?!1:(zl.crossVectors(Sr,Mr),t=[zl.x,zl.y,zl.z],Zf(t,Hs,Vs,Gs,Bl))}clampPoint(e,t){return t.copy(e).clamp(this.min,this.max)}distanceToPoint(e){return this.clampPoint(e,hi).distanceTo(e)}getBoundingSphere(e){return this.isEmpty()?e.makeEmpty():(this.getCenter(e.center),e.radius=this.getSize(hi).length()*.5),e}intersect(e){return this.min.max(e.min),this.max.min(e.max),this.isEmpty()&&this.makeEmpty(),this}union(e){return this.min.min(e.min),this.max.max(e.max),this}applyMatrix4(e){return this.isEmpty()?this:(zi[0].set(this.min.x,this.min.y,this.min.z).applyMatrix4(e),zi[1].set(this.min.x,this.min.y,this.max.z).applyMatrix4(e),zi[2].set(this.min.x,this.max.y,this.min.z).applyMatrix4(e),zi[3].set(this.min.x,this.max.y,this.max.z).applyMatrix4(e),zi[4].set(this.max.x,this.min.y,this.min.z).applyMatrix4(e),zi[5].set(this.max.x,this.min.y,this.max.z).applyMatrix4(e),zi[6].set(this.max.x,this.max.y,this.min.z).applyMatrix4(e),zi[7].set(this.max.x,this.max.y,this.max.z).applyMatrix4(e),this.setFromPoints(zi),this)}translate(e){return this.min.add(e),this.max.add(e),this}equals(e){return e.min.equals(this.min)&&e.max.equals(this.max)}}const zi=[new W,new W,new W,new W,new W,new W,new W,new W],hi=new W,Ol=new Ca,Hs=new W,Vs=new W,Gs=new W,Sr=new W,Mr=new W,Kr=new W,aa=new W,Bl=new W,zl=new W,$r=new W;function Zf(s,e,t,i,o){for(let a=0,u=s.length-3;a<=u;a+=3){$r.fromArray(s,a);const c=o.x*Math.abs($r.x)+o.y*Math.abs($r.y)+o.z*Math.abs($r.z),h=e.dot($r),d=t.dot($r),m=i.dot($r);if(Math.max(-Math.max(h,d,m),Math.min(h,d,m))>c)return!1}return!0}const CS=new Ca,la=new W,Jf=new W;class Iu{constructor(e=new W,t=-1){this.isSphere=!0,this.center=e,this.radius=t}set(e,t){return this.center.copy(e),this.radius=t,this}setFromPoints(e,t){const i=this.center;t!==void 0?i.copy(t):CS.setFromPoints(e).getCenter(i);let o=0;for(let a=0,u=e.length;a<u;a++)o=Math.max(o,i.distanceToSquared(e[a]));return this.radius=Math.sqrt(o),this}copy(e){return this.center.copy(e.center),this.radius=e.radius,this}isEmpty(){return this.radius<0}makeEmpty(){return this.center.set(0,0,0),this.radius=-1,this}containsPoint(e){return e.distanceToSquared(this.center)<=this.radius*this.radius}distanceToPoint(e){return e.distanceTo(this.center)-this.radius}intersectsSphere(e){const t=this.radius+e.radius;return e.center.distanceToSquared(this.center)<=t*t}intersectsBox(e){return e.intersectsSphere(this)}intersectsPlane(e){return Math.abs(e.distanceToPoint(this.center))<=this.radius}clampPoint(e,t){const i=this.center.distanceToSquared(e);return t.copy(e),i>this.radius*this.radius&&(t.sub(this.center).normalize(),t.multiplyScalar(this.radius).add(this.center)),t}getBoundingBox(e){return this.isEmpty()?(e.makeEmpty(),e):(e.set(this.center,this.center),e.expandByScalar(this.radius),e)}applyMatrix4(e){return this.center.applyMatrix4(e),this.radius=this.radius*e.getMaxScaleOnAxis(),this}translate(e){return this.center.add(e),this}expandByPoint(e){if(this.isEmpty())return this.center.copy(e),this.radius=0,this;la.subVectors(e,this.center);const t=la.lengthSq();if(t>this.radius*this.radius){const i=Math.sqrt(t),o=(i-this.radius)*.5;this.center.addScaledVector(la,o/i),this.radius+=o}return this}union(e){return e.isEmpty()?this:this.isEmpty()?(this.copy(e),this):(this.center.equals(e.center)===!0?this.radius=Math.max(this.radius,e.radius):(Jf.subVectors(e.center,this.center).setLength(e.radius),this.expandByPoint(la.copy(e.center).add(Jf)),this.expandByPoint(la.copy(e.center).sub(Jf))),this)}equals(e){return e.center.equals(this.center)&&e.radius===this.radius}clone(){return new this.constructor().copy(this)}}const ki=new W,Qf=new W,kl=new W,Er=new W,eh=new W,Hl=new W,th=new W;class Sd{constructor(e=new W,t=new W(0,0,-1)){this.origin=e,this.direction=t}set(e,t){return this.origin.copy(e),this.direction.copy(t),this}copy(e){return this.origin.copy(e.origin),this.direction.copy(e.direction),this}at(e,t){return t.copy(this.origin).addScaledVector(this.direction,e)}lookAt(e){return this.direction.copy(e).sub(this.origin).normalize(),this}recast(e){return this.origin.copy(this.at(e,ki)),this}closestPointToPoint(e,t){t.subVectors(e,this.origin);const i=t.dot(this.direction);return i<0?t.copy(this.origin):t.copy(this.origin).addScaledVector(this.direction,i)}distanceToPoint(e){return Math.sqrt(this.distanceSqToPoint(e))}distanceSqToPoint(e){const t=ki.subVectors(e,this.origin).dot(this.direction);return t<0?this.origin.distanceToSquared(e):(ki.copy(this.origin).addScaledVector(this.direction,t),ki.distanceToSquared(e))}distanceSqToSegment(e,t,i,o){Qf.copy(e).add(t).multiplyScalar(.5),kl.copy(t).sub(e).normalize(),Er.copy(this.origin).sub(Qf);const a=e.distanceTo(t)*.5,u=-this.direction.dot(kl),c=Er.dot(this.direction),h=-Er.dot(kl),d=Er.lengthSq(),m=Math.abs(1-u*u);let _,g,S,M;if(m>0)if(_=u*h-c,g=u*c-h,M=a*m,_>=0)if(g>=-M)if(g<=M){const E=1/m;_*=E,g*=E,S=_*(_+u*g+2*c)+g*(u*_+g+2*h)+d}else g=a,_=Math.max(0,-(u*g+c)),S=-_*_+g*(g+2*h)+d;else g=-a,_=Math.max(0,-(u*g+c)),S=-_*_+g*(g+2*h)+d;else g<=-M?(_=Math.max(0,-(-u*a+c)),g=_>0?-a:Math.min(Math.max(-a,-h),a),S=-_*_+g*(g+2*h)+d):g<=M?(_=0,g=Math.min(Math.max(-a,-h),a),S=g*(g+2*h)+d):(_=Math.max(0,-(u*a+c)),g=_>0?a:Math.min(Math.max(-a,-h),a),S=-_*_+g*(g+2*h)+d);else g=u>0?-a:a,_=Math.max(0,-(u*g+c)),S=-_*_+g*(g+2*h)+d;return i&&i.copy(this.origin).addScaledVector(this.direction,_),o&&o.copy(Qf).addScaledVector(kl,g),S}intersectSphere(e,t){ki.subVectors(e.center,this.origin);const i=ki.dot(this.direction),o=ki.dot(ki)-i*i,a=e.radius*e.radius;if(o>a)return null;const u=Math.sqrt(a-o),c=i-u,h=i+u;return h<0?null:c<0?this.at(h,t):this.at(c,t)}intersectsSphere(e){return this.distanceSqToPoint(e.center)<=e.radius*e.radius}distanceToPlane(e){const t=e.normal.dot(this.direction);if(t===0)return e.distanceToPoint(this.origin)===0?0:null;const i=-(this.origin.dot(e.normal)+e.constant)/t;return i>=0?i:null}intersectPlane(e,t){const i=this.distanceToPlane(e);return i===null?null:this.at(i,t)}intersectsPlane(e){const t=e.distanceToPoint(this.origin);return t===0||e.normal.dot(this.direction)*t<0}intersectBox(e,t){let i,o,a,u,c,h;const d=1/this.direction.x,m=1/this.direction.y,_=1/this.direction.z,g=this.origin;return d>=0?(i=(e.min.x-g.x)*d,o=(e.max.x-g.x)*d):(i=(e.max.x-g.x)*d,o=(e.min.x-g.x)*d),m>=0?(a=(e.min.y-g.y)*m,u=(e.max.y-g.y)*m):(a=(e.max.y-g.y)*m,u=(e.min.y-g.y)*m),i>u||a>o||((a>i||isNaN(i))&&(i=a),(u<o||isNaN(o))&&(o=u),_>=0?(c=(e.min.z-g.z)*_,h=(e.max.z-g.z)*_):(c=(e.max.z-g.z)*_,h=(e.min.z-g.z)*_),i>h||c>o)||((c>i||i!==i)&&(i=c),(h<o||o!==o)&&(o=h),o<0)?null:this.at(i>=0?i:o,t)}intersectsBox(e){return this.intersectBox(e,ki)!==null}intersectTriangle(e,t,i,o,a){eh.subVectors(t,e),Hl.subVectors(i,e),th.crossVectors(eh,Hl);let u=this.direction.dot(th),c;if(u>0){if(o)return null;c=1}else if(u<0)c=-1,u=-u;else return null;Er.subVectors(this.origin,e);const h=c*this.direction.dot(Hl.crossVectors(Er,Hl));if(h<0)return null;const d=c*this.direction.dot(eh.cross(Er));if(d<0||h+d>u)return null;const m=-c*Er.dot(th);return m<0?null:this.at(m/u,a)}applyMatrix4(e){return this.origin.applyMatrix4(e),this.direction.transformDirection(e),this}equals(e){return e.origin.equals(this.origin)&&e.direction.equals(this.direction)}clone(){return new this.constructor().copy(this)}}class kt{constructor(e,t,i,o,a,u,c,h,d,m,_,g,S,M,E,y){kt.prototype.isMatrix4=!0,this.elements=[1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1],e!==void 0&&this.set(e,t,i,o,a,u,c,h,d,m,_,g,S,M,E,y)}set(e,t,i,o,a,u,c,h,d,m,_,g,S,M,E,y){const x=this.elements;return x[0]=e,x[4]=t,x[8]=i,x[12]=o,x[1]=a,x[5]=u,x[9]=c,x[13]=h,x[2]=d,x[6]=m,x[10]=_,x[14]=g,x[3]=S,x[7]=M,x[11]=E,x[15]=y,this}identity(){return this.set(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1),this}clone(){return new kt().fromArray(this.elements)}copy(e){const t=this.elements,i=e.elements;return t[0]=i[0],t[1]=i[1],t[2]=i[2],t[3]=i[3],t[4]=i[4],t[5]=i[5],t[6]=i[6],t[7]=i[7],t[8]=i[8],t[9]=i[9],t[10]=i[10],t[11]=i[11],t[12]=i[12],t[13]=i[13],t[14]=i[14],t[15]=i[15],this}copyPosition(e){const t=this.elements,i=e.elements;return t[12]=i[12],t[13]=i[13],t[14]=i[14],this}setFromMatrix3(e){const t=e.elements;return this.set(t[0],t[3],t[6],0,t[1],t[4],t[7],0,t[2],t[5],t[8],0,0,0,0,1),this}extractBasis(e,t,i){return e.setFromMatrixColumn(this,0),t.setFromMatrixColumn(this,1),i.setFromMatrixColumn(this,2),this}makeBasis(e,t,i){return this.set(e.x,t.x,i.x,0,e.y,t.y,i.y,0,e.z,t.z,i.z,0,0,0,0,1),this}extractRotation(e){const t=this.elements,i=e.elements,o=1/Ws.setFromMatrixColumn(e,0).length(),a=1/Ws.setFromMatrixColumn(e,1).length(),u=1/Ws.setFromMatrixColumn(e,2).length();return t[0]=i[0]*o,t[1]=i[1]*o,t[2]=i[2]*o,t[3]=0,t[4]=i[4]*a,t[5]=i[5]*a,t[6]=i[6]*a,t[7]=0,t[8]=i[8]*u,t[9]=i[9]*u,t[10]=i[10]*u,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,this}makeRotationFromEuler(e){const t=this.elements,i=e.x,o=e.y,a=e.z,u=Math.cos(i),c=Math.sin(i),h=Math.cos(o),d=Math.sin(o),m=Math.cos(a),_=Math.sin(a);if(e.order==="XYZ"){const g=u*m,S=u*_,M=c*m,E=c*_;t[0]=h*m,t[4]=-h*_,t[8]=d,t[1]=S+M*d,t[5]=g-E*d,t[9]=-c*h,t[2]=E-g*d,t[6]=M+S*d,t[10]=u*h}else if(e.order==="YXZ"){const g=h*m,S=h*_,M=d*m,E=d*_;t[0]=g+E*c,t[4]=M*c-S,t[8]=u*d,t[1]=u*_,t[5]=u*m,t[9]=-c,t[2]=S*c-M,t[6]=E+g*c,t[10]=u*h}else if(e.order==="ZXY"){const g=h*m,S=h*_,M=d*m,E=d*_;t[0]=g-E*c,t[4]=-u*_,t[8]=M+S*c,t[1]=S+M*c,t[5]=u*m,t[9]=E-g*c,t[2]=-u*d,t[6]=c,t[10]=u*h}else if(e.order==="ZYX"){const g=u*m,S=u*_,M=c*m,E=c*_;t[0]=h*m,t[4]=M*d-S,t[8]=g*d+E,t[1]=h*_,t[5]=E*d+g,t[9]=S*d-M,t[2]=-d,t[6]=c*h,t[10]=u*h}else if(e.order==="YZX"){const g=u*h,S=u*d,M=c*h,E=c*d;t[0]=h*m,t[4]=E-g*_,t[8]=M*_+S,t[1]=_,t[5]=u*m,t[9]=-c*m,t[2]=-d*m,t[6]=S*_+M,t[10]=g-E*_}else if(e.order==="XZY"){const g=u*h,S=u*d,M=c*h,E=c*d;t[0]=h*m,t[4]=-_,t[8]=d*m,t[1]=g*_+E,t[5]=u*m,t[9]=S*_-M,t[2]=M*_-S,t[6]=c*m,t[10]=E*_+g}return t[3]=0,t[7]=0,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,this}makeRotationFromQuaternion(e){return this.compose(RS,e,PS)}lookAt(e,t,i){const o=this.elements;return qn.subVectors(e,t),qn.lengthSq()===0&&(qn.z=1),qn.normalize(),wr.crossVectors(i,qn),wr.lengthSq()===0&&(Math.abs(i.z)===1?qn.x+=1e-4:qn.z+=1e-4,qn.normalize(),wr.crossVectors(i,qn)),wr.normalize(),Vl.crossVectors(qn,wr),o[0]=wr.x,o[4]=Vl.x,o[8]=qn.x,o[1]=wr.y,o[5]=Vl.y,o[9]=qn.y,o[2]=wr.z,o[6]=Vl.z,o[10]=qn.z,this}multiply(e){return this.multiplyMatrices(this,e)}premultiply(e){return this.multiplyMatrices(e,this)}multiplyMatrices(e,t){const i=e.elements,o=t.elements,a=this.elements,u=i[0],c=i[4],h=i[8],d=i[12],m=i[1],_=i[5],g=i[9],S=i[13],M=i[2],E=i[6],y=i[10],x=i[14],b=i[3],T=i[7],P=i[11],H=i[15],D=o[0],N=o[4],z=o[8],C=o[12],w=o[1],U=o[5],X=o[9],k=o[13],Y=o[2],ae=o[6],te=o[10],ce=o[14],G=o[3],he=o[7],re=o[11],O=o[15];return a[0]=u*D+c*w+h*Y+d*G,a[4]=u*N+c*U+h*ae+d*he,a[8]=u*z+c*X+h*te+d*re,a[12]=u*C+c*k+h*ce+d*O,a[1]=m*D+_*w+g*Y+S*G,a[5]=m*N+_*U+g*ae+S*he,a[9]=m*z+_*X+g*te+S*re,a[13]=m*C+_*k+g*ce+S*O,a[2]=M*D+E*w+y*Y+x*G,a[6]=M*N+E*U+y*ae+x*he,a[10]=M*z+E*X+y*te+x*re,a[14]=M*C+E*k+y*ce+x*O,a[3]=b*D+T*w+P*Y+H*G,a[7]=b*N+T*U+P*ae+H*he,a[11]=b*z+T*X+P*te+H*re,a[15]=b*C+T*k+P*ce+H*O,this}multiplyScalar(e){const t=this.elements;return t[0]*=e,t[4]*=e,t[8]*=e,t[12]*=e,t[1]*=e,t[5]*=e,t[9]*=e,t[13]*=e,t[2]*=e,t[6]*=e,t[10]*=e,t[14]*=e,t[3]*=e,t[7]*=e,t[11]*=e,t[15]*=e,this}determinant(){const e=this.elements,t=e[0],i=e[4],o=e[8],a=e[12],u=e[1],c=e[5],h=e[9],d=e[13],m=e[2],_=e[6],g=e[10],S=e[14],M=e[3],E=e[7],y=e[11],x=e[15];return M*(+a*h*_-o*d*_-a*c*g+i*d*g+o*c*S-i*h*S)+E*(+t*h*S-t*d*g+a*u*g-o*u*S+o*d*m-a*h*m)+y*(+t*d*_-t*c*S-a*u*_+i*u*S+a*c*m-i*d*m)+x*(-o*c*m-t*h*_+t*c*g+o*u*_-i*u*g+i*h*m)}transpose(){const e=this.elements;let t;return t=e[1],e[1]=e[4],e[4]=t,t=e[2],e[2]=e[8],e[8]=t,t=e[6],e[6]=e[9],e[9]=t,t=e[3],e[3]=e[12],e[12]=t,t=e[7],e[7]=e[13],e[13]=t,t=e[11],e[11]=e[14],e[14]=t,this}setPosition(e,t,i){const o=this.elements;return e.isVector3?(o[12]=e.x,o[13]=e.y,o[14]=e.z):(o[12]=e,o[13]=t,o[14]=i),this}invert(){const e=this.elements,t=e[0],i=e[1],o=e[2],a=e[3],u=e[4],c=e[5],h=e[6],d=e[7],m=e[8],_=e[9],g=e[10],S=e[11],M=e[12],E=e[13],y=e[14],x=e[15],b=_*y*d-E*g*d+E*h*S-c*y*S-_*h*x+c*g*x,T=M*g*d-m*y*d-M*h*S+u*y*S+m*h*x-u*g*x,P=m*E*d-M*_*d+M*c*S-u*E*S-m*c*x+u*_*x,H=M*_*h-m*E*h-M*c*g+u*E*g+m*c*y-u*_*y,D=t*b+i*T+o*P+a*H;if(D===0)return this.set(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);const N=1/D;return e[0]=b*N,e[1]=(E*g*a-_*y*a-E*o*S+i*y*S+_*o*x-i*g*x)*N,e[2]=(c*y*a-E*h*a+E*o*d-i*y*d-c*o*x+i*h*x)*N,e[3]=(_*h*a-c*g*a-_*o*d+i*g*d+c*o*S-i*h*S)*N,e[4]=T*N,e[5]=(m*y*a-M*g*a+M*o*S-t*y*S-m*o*x+t*g*x)*N,e[6]=(M*h*a-u*y*a-M*o*d+t*y*d+u*o*x-t*h*x)*N,e[7]=(u*g*a-m*h*a+m*o*d-t*g*d-u*o*S+t*h*S)*N,e[8]=P*N,e[9]=(M*_*a-m*E*a-M*i*S+t*E*S+m*i*x-t*_*x)*N,e[10]=(u*E*a-M*c*a+M*i*d-t*E*d-u*i*x+t*c*x)*N,e[11]=(m*c*a-u*_*a-m*i*d+t*_*d+u*i*S-t*c*S)*N,e[12]=H*N,e[13]=(m*E*o-M*_*o+M*i*g-t*E*g-m*i*y+t*_*y)*N,e[14]=(M*c*o-u*E*o-M*i*h+t*E*h+u*i*y-t*c*y)*N,e[15]=(u*_*o-m*c*o+m*i*h-t*_*h-u*i*g+t*c*g)*N,this}scale(e){const t=this.elements,i=e.x,o=e.y,a=e.z;return t[0]*=i,t[4]*=o,t[8]*=a,t[1]*=i,t[5]*=o,t[9]*=a,t[2]*=i,t[6]*=o,t[10]*=a,t[3]*=i,t[7]*=o,t[11]*=a,this}getMaxScaleOnAxis(){const e=this.elements,t=e[0]*e[0]+e[1]*e[1]+e[2]*e[2],i=e[4]*e[4]+e[5]*e[5]+e[6]*e[6],o=e[8]*e[8]+e[9]*e[9]+e[10]*e[10];return Math.sqrt(Math.max(t,i,o))}makeTranslation(e,t,i){return e.isVector3?this.set(1,0,0,e.x,0,1,0,e.y,0,0,1,e.z,0,0,0,1):this.set(1,0,0,e,0,1,0,t,0,0,1,i,0,0,0,1),this}makeRotationX(e){const t=Math.cos(e),i=Math.sin(e);return this.set(1,0,0,0,0,t,-i,0,0,i,t,0,0,0,0,1),this}makeRotationY(e){const t=Math.cos(e),i=Math.sin(e);return this.set(t,0,i,0,0,1,0,0,-i,0,t,0,0,0,0,1),this}makeRotationZ(e){const t=Math.cos(e),i=Math.sin(e);return this.set(t,-i,0,0,i,t,0,0,0,0,1,0,0,0,0,1),this}makeRotationAxis(e,t){const i=Math.cos(t),o=Math.sin(t),a=1-i,u=e.x,c=e.y,h=e.z,d=a*u,m=a*c;return this.set(d*u+i,d*c-o*h,d*h+o*c,0,d*c+o*h,m*c+i,m*h-o*u,0,d*h-o*c,m*h+o*u,a*h*h+i,0,0,0,0,1),this}makeScale(e,t,i){return this.set(e,0,0,0,0,t,0,0,0,0,i,0,0,0,0,1),this}makeShear(e,t,i,o,a,u){return this.set(1,i,a,0,e,1,u,0,t,o,1,0,0,0,0,1),this}compose(e,t,i){const o=this.elements,a=t._x,u=t._y,c=t._z,h=t._w,d=a+a,m=u+u,_=c+c,g=a*d,S=a*m,M=a*_,E=u*m,y=u*_,x=c*_,b=h*d,T=h*m,P=h*_,H=i.x,D=i.y,N=i.z;return o[0]=(1-(E+x))*H,o[1]=(S+P)*H,o[2]=(M-T)*H,o[3]=0,o[4]=(S-P)*D,o[5]=(1-(g+x))*D,o[6]=(y+b)*D,o[7]=0,o[8]=(M+T)*N,o[9]=(y-b)*N,o[10]=(1-(g+E))*N,o[11]=0,o[12]=e.x,o[13]=e.y,o[14]=e.z,o[15]=1,this}decompose(e,t,i){const o=this.elements;let a=Ws.set(o[0],o[1],o[2]).length();const u=Ws.set(o[4],o[5],o[6]).length(),c=Ws.set(o[8],o[9],o[10]).length();this.determinant()<0&&(a=-a),e.x=o[12],e.y=o[13],e.z=o[14],di.copy(this);const d=1/a,m=1/u,_=1/c;return di.elements[0]*=d,di.elements[1]*=d,di.elements[2]*=d,di.elements[4]*=m,di.elements[5]*=m,di.elements[6]*=m,di.elements[8]*=_,di.elements[9]*=_,di.elements[10]*=_,t.setFromRotationMatrix(di),i.x=a,i.y=u,i.z=c,this}makePerspective(e,t,i,o,a,u,c=ji){const h=this.elements,d=2*a/(t-e),m=2*a/(i-o),_=(t+e)/(t-e),g=(i+o)/(i-o);let S,M;if(c===ji)S=-(u+a)/(u-a),M=-2*u*a/(u-a);else if(c===Cu)S=-u/(u-a),M=-u*a/(u-a);else throw new Error("THREE.Matrix4.makePerspective(): Invalid coordinate system: "+c);return h[0]=d,h[4]=0,h[8]=_,h[12]=0,h[1]=0,h[5]=m,h[9]=g,h[13]=0,h[2]=0,h[6]=0,h[10]=S,h[14]=M,h[3]=0,h[7]=0,h[11]=-1,h[15]=0,this}makeOrthographic(e,t,i,o,a,u,c=ji){const h=this.elements,d=1/(t-e),m=1/(i-o),_=1/(u-a),g=(t+e)*d,S=(i+o)*m;let M,E;if(c===ji)M=(u+a)*_,E=-2*_;else if(c===Cu)M=a*_,E=-1*_;else throw new Error("THREE.Matrix4.makeOrthographic(): Invalid coordinate system: "+c);return h[0]=2*d,h[4]=0,h[8]=0,h[12]=-g,h[1]=0,h[5]=2*m,h[9]=0,h[13]=-S,h[2]=0,h[6]=0,h[10]=E,h[14]=-M,h[3]=0,h[7]=0,h[11]=0,h[15]=1,this}equals(e){const t=this.elements,i=e.elements;for(let o=0;o<16;o++)if(t[o]!==i[o])return!1;return!0}fromArray(e,t=0){for(let i=0;i<16;i++)this.elements[i]=e[i+t];return this}toArray(e=[],t=0){const i=this.elements;return e[t]=i[0],e[t+1]=i[1],e[t+2]=i[2],e[t+3]=i[3],e[t+4]=i[4],e[t+5]=i[5],e[t+6]=i[6],e[t+7]=i[7],e[t+8]=i[8],e[t+9]=i[9],e[t+10]=i[10],e[t+11]=i[11],e[t+12]=i[12],e[t+13]=i[13],e[t+14]=i[14],e[t+15]=i[15],e}}const Ws=new W,di=new kt,RS=new W(0,0,0),PS=new W(1,1,1),wr=new W,Vl=new W,qn=new W,$g=new kt,Zg=new ls;class Ai{constructor(e=0,t=0,i=0,o=Ai.DEFAULT_ORDER){this.isEuler=!0,this._x=e,this._y=t,this._z=i,this._order=o}get x(){return this._x}set x(e){this._x=e,this._onChangeCallback()}get y(){return this._y}set y(e){this._y=e,this._onChangeCallback()}get z(){return this._z}set z(e){this._z=e,this._onChangeCallback()}get order(){return this._order}set order(e){this._order=e,this._onChangeCallback()}set(e,t,i,o=this._order){return this._x=e,this._y=t,this._z=i,this._order=o,this._onChangeCallback(),this}clone(){return new this.constructor(this._x,this._y,this._z,this._order)}copy(e){return this._x=e._x,this._y=e._y,this._z=e._z,this._order=e._order,this._onChangeCallback(),this}setFromRotationMatrix(e,t=this._order,i=!0){const o=e.elements,a=o[0],u=o[4],c=o[8],h=o[1],d=o[5],m=o[9],_=o[2],g=o[6],S=o[10];switch(t){case"XYZ":this._y=Math.asin(pn(c,-1,1)),Math.abs(c)<.9999999?(this._x=Math.atan2(-m,S),this._z=Math.atan2(-u,a)):(this._x=Math.atan2(g,d),this._z=0);break;case"YXZ":this._x=Math.asin(-pn(m,-1,1)),Math.abs(m)<.9999999?(this._y=Math.atan2(c,S),this._z=Math.atan2(h,d)):(this._y=Math.atan2(-_,a),this._z=0);break;case"ZXY":this._x=Math.asin(pn(g,-1,1)),Math.abs(g)<.9999999?(this._y=Math.atan2(-_,S),this._z=Math.atan2(-u,d)):(this._y=0,this._z=Math.atan2(h,a));break;case"ZYX":this._y=Math.asin(-pn(_,-1,1)),Math.abs(_)<.9999999?(this._x=Math.atan2(g,S),this._z=Math.atan2(h,a)):(this._x=0,this._z=Math.atan2(-u,d));break;case"YZX":this._z=Math.asin(pn(h,-1,1)),Math.abs(h)<.9999999?(this._x=Math.atan2(-m,d),this._y=Math.atan2(-_,a)):(this._x=0,this._y=Math.atan2(c,S));break;case"XZY":this._z=Math.asin(-pn(u,-1,1)),Math.abs(u)<.9999999?(this._x=Math.atan2(g,d),this._y=Math.atan2(c,a)):(this._x=Math.atan2(-m,S),this._y=0);break;default:console.warn("THREE.Euler: .setFromRotationMatrix() encountered an unknown order: "+t)}return this._order=t,i===!0&&this._onChangeCallback(),this}setFromQuaternion(e,t,i){return $g.makeRotationFromQuaternion(e),this.setFromRotationMatrix($g,t,i)}setFromVector3(e,t=this._order){return this.set(e.x,e.y,e.z,t)}reorder(e){return Zg.setFromEuler(this),this.setFromQuaternion(Zg,e)}equals(e){return e._x===this._x&&e._y===this._y&&e._z===this._z&&e._order===this._order}fromArray(e){return this._x=e[0],this._y=e[1],this._z=e[2],e[3]!==void 0&&(this._order=e[3]),this._onChangeCallback(),this}toArray(e=[],t=0){return e[t]=this._x,e[t+1]=this._y,e[t+2]=this._z,e[t+3]=this._order,e}_onChange(e){return this._onChangeCallback=e,this}_onChangeCallback(){}*[Symbol.iterator](){yield this._x,yield this._y,yield this._z,yield this._order}}Ai.DEFAULT_ORDER="XYZ";class A_{constructor(){this.mask=1}set(e){this.mask=(1<<e|0)>>>0}enable(e){this.mask|=1<<e|0}enableAll(){this.mask=-1}toggle(e){this.mask^=1<<e|0}disable(e){this.mask&=~(1<<e|0)}disableAll(){this.mask=0}test(e){return(this.mask&e.mask)!==0}isEnabled(e){return(this.mask&(1<<e|0))!==0}}let bS=0;const Jg=new W,Xs=new ls,Hi=new kt,Gl=new W,ua=new W,LS=new W,DS=new ls,Qg=new W(1,0,0),e0=new W(0,1,0),t0=new W(0,0,1),n0={type:"added"},NS={type:"removed"},js={type:"childadded",child:null},nh={type:"childremoved",child:null};class cn extends hs{constructor(){super(),this.isObject3D=!0,Object.defineProperty(this,"id",{value:bS++}),this.uuid=Yi(),this.name="",this.type="Object3D",this.parent=null,this.children=[],this.up=cn.DEFAULT_UP.clone();const e=new W,t=new Ai,i=new ls,o=new W(1,1,1);function a(){i.setFromEuler(t,!1)}function u(){t.setFromQuaternion(i,void 0,!1)}t._onChange(a),i._onChange(u),Object.defineProperties(this,{position:{configurable:!0,enumerable:!0,value:e},rotation:{configurable:!0,enumerable:!0,value:t},quaternion:{configurable:!0,enumerable:!0,value:i},scale:{configurable:!0,enumerable:!0,value:o},modelViewMatrix:{value:new kt},normalMatrix:{value:new pt}}),this.matrix=new kt,this.matrixWorld=new kt,this.matrixAutoUpdate=cn.DEFAULT_MATRIX_AUTO_UPDATE,this.matrixWorldAutoUpdate=cn.DEFAULT_MATRIX_WORLD_AUTO_UPDATE,this.matrixWorldNeedsUpdate=!1,this.layers=new A_,this.visible=!0,this.castShadow=!1,this.receiveShadow=!1,this.frustumCulled=!0,this.renderOrder=0,this.animations=[],this.userData={}}onBeforeShadow(){}onAfterShadow(){}onBeforeRender(){}onAfterRender(){}applyMatrix4(e){this.matrixAutoUpdate&&this.updateMatrix(),this.matrix.premultiply(e),this.matrix.decompose(this.position,this.quaternion,this.scale)}applyQuaternion(e){return this.quaternion.premultiply(e),this}setRotationFromAxisAngle(e,t){this.quaternion.setFromAxisAngle(e,t)}setRotationFromEuler(e){this.quaternion.setFromEuler(e,!0)}setRotationFromMatrix(e){this.quaternion.setFromRotationMatrix(e)}setRotationFromQuaternion(e){this.quaternion.copy(e)}rotateOnAxis(e,t){return Xs.setFromAxisAngle(e,t),this.quaternion.multiply(Xs),this}rotateOnWorldAxis(e,t){return Xs.setFromAxisAngle(e,t),this.quaternion.premultiply(Xs),this}rotateX(e){return this.rotateOnAxis(Qg,e)}rotateY(e){return this.rotateOnAxis(e0,e)}rotateZ(e){return this.rotateOnAxis(t0,e)}translateOnAxis(e,t){return Jg.copy(e).applyQuaternion(this.quaternion),this.position.add(Jg.multiplyScalar(t)),this}translateX(e){return this.translateOnAxis(Qg,e)}translateY(e){return this.translateOnAxis(e0,e)}translateZ(e){return this.translateOnAxis(t0,e)}localToWorld(e){return this.updateWorldMatrix(!0,!1),e.applyMatrix4(this.matrixWorld)}worldToLocal(e){return this.updateWorldMatrix(!0,!1),e.applyMatrix4(Hi.copy(this.matrixWorld).invert())}lookAt(e,t,i){e.isVector3?Gl.copy(e):Gl.set(e,t,i);const o=this.parent;this.updateWorldMatrix(!0,!1),ua.setFromMatrixPosition(this.matrixWorld),this.isCamera||this.isLight?Hi.lookAt(ua,Gl,this.up):Hi.lookAt(Gl,ua,this.up),this.quaternion.setFromRotationMatrix(Hi),o&&(Hi.extractRotation(o.matrixWorld),Xs.setFromRotationMatrix(Hi),this.quaternion.premultiply(Xs.invert()))}add(e){if(arguments.length>1){for(let t=0;t<arguments.length;t++)this.add(arguments[t]);return this}return e===this?(console.error("THREE.Object3D.add: object can't be added as a child of itself.",e),this):(e&&e.isObject3D?(e.removeFromParent(),e.parent=this,this.children.push(e),e.dispatchEvent(n0),js.child=e,this.dispatchEvent(js),js.child=null):console.error("THREE.Object3D.add: object not an instance of THREE.Object3D.",e),this)}remove(e){if(arguments.length>1){for(let i=0;i<arguments.length;i++)this.remove(arguments[i]);return this}const t=this.children.indexOf(e);return t!==-1&&(e.parent=null,this.children.splice(t,1),e.dispatchEvent(NS),nh.child=e,this.dispatchEvent(nh),nh.child=null),this}removeFromParent(){const e=this.parent;return e!==null&&e.remove(this),this}clear(){return this.remove(...this.children)}attach(e){return this.updateWorldMatrix(!0,!1),Hi.copy(this.matrixWorld).invert(),e.parent!==null&&(e.parent.updateWorldMatrix(!0,!1),Hi.multiply(e.parent.matrixWorld)),e.applyMatrix4(Hi),e.removeFromParent(),e.parent=this,this.children.push(e),e.updateWorldMatrix(!1,!0),e.dispatchEvent(n0),js.child=e,this.dispatchEvent(js),js.child=null,this}getObjectById(e){return this.getObjectByProperty("id",e)}getObjectByName(e){return this.getObjectByProperty("name",e)}getObjectByProperty(e,t){if(this[e]===t)return this;for(let i=0,o=this.children.length;i<o;i++){const u=this.children[i].getObjectByProperty(e,t);if(u!==void 0)return u}}getObjectsByProperty(e,t,i=[]){this[e]===t&&i.push(this);const o=this.children;for(let a=0,u=o.length;a<u;a++)o[a].getObjectsByProperty(e,t,i);return i}getWorldPosition(e){return this.updateWorldMatrix(!0,!1),e.setFromMatrixPosition(this.matrixWorld)}getWorldQuaternion(e){return this.updateWorldMatrix(!0,!1),this.matrixWorld.decompose(ua,e,LS),e}getWorldScale(e){return this.updateWorldMatrix(!0,!1),this.matrixWorld.decompose(ua,DS,e),e}getWorldDirection(e){this.updateWorldMatrix(!0,!1);const t=this.matrixWorld.elements;return e.set(t[8],t[9],t[10]).normalize()}raycast(){}traverse(e){e(this);const t=this.children;for(let i=0,o=t.length;i<o;i++)t[i].traverse(e)}traverseVisible(e){if(this.visible===!1)return;e(this);const t=this.children;for(let i=0,o=t.length;i<o;i++)t[i].traverseVisible(e)}traverseAncestors(e){const t=this.parent;t!==null&&(e(t),t.traverseAncestors(e))}updateMatrix(){this.matrix.compose(this.position,this.quaternion,this.scale),this.matrixWorldNeedsUpdate=!0}updateMatrixWorld(e){this.matrixAutoUpdate&&this.updateMatrix(),(this.matrixWorldNeedsUpdate||e)&&(this.matrixWorldAutoUpdate===!0&&(this.parent===null?this.matrixWorld.copy(this.matrix):this.matrixWorld.multiplyMatrices(this.parent.matrixWorld,this.matrix)),this.matrixWorldNeedsUpdate=!1,e=!0);const t=this.children;for(let i=0,o=t.length;i<o;i++)t[i].updateMatrixWorld(e)}updateWorldMatrix(e,t){const i=this.parent;if(e===!0&&i!==null&&i.updateWorldMatrix(!0,!1),this.matrixAutoUpdate&&this.updateMatrix(),this.matrixWorldAutoUpdate===!0&&(this.parent===null?this.matrixWorld.copy(this.matrix):this.matrixWorld.multiplyMatrices(this.parent.matrixWorld,this.matrix)),t===!0){const o=this.children;for(let a=0,u=o.length;a<u;a++)o[a].updateWorldMatrix(!1,!0)}}toJSON(e){const t=e===void 0||typeof e=="string",i={};t&&(e={geometries:{},materials:{},textures:{},images:{},shapes:{},skeletons:{},animations:{},nodes:{}},i.metadata={version:4.6,type:"Object",generator:"Object3D.toJSON"});const o={};o.uuid=this.uuid,o.type=this.type,this.name!==""&&(o.name=this.name),this.castShadow===!0&&(o.castShadow=!0),this.receiveShadow===!0&&(o.receiveShadow=!0),this.visible===!1&&(o.visible=!1),this.frustumCulled===!1&&(o.frustumCulled=!1),this.renderOrder!==0&&(o.renderOrder=this.renderOrder),Object.keys(this.userData).length>0&&(o.userData=this.userData),o.layers=this.layers.mask,o.matrix=this.matrix.toArray(),o.up=this.up.toArray(),this.matrixAutoUpdate===!1&&(o.matrixAutoUpdate=!1),this.isInstancedMesh&&(o.type="InstancedMesh",o.count=this.count,o.instanceMatrix=this.instanceMatrix.toJSON(),this.instanceColor!==null&&(o.instanceColor=this.instanceColor.toJSON())),this.isBatchedMesh&&(o.type="BatchedMesh",o.perObjectFrustumCulled=this.perObjectFrustumCulled,o.sortObjects=this.sortObjects,o.drawRanges=this._drawRanges,o.reservedRanges=this._reservedRanges,o.visibility=this._visibility,o.active=this._active,o.bounds=this._bounds.map(c=>({boxInitialized:c.boxInitialized,boxMin:c.box.min.toArray(),boxMax:c.box.max.toArray(),sphereInitialized:c.sphereInitialized,sphereRadius:c.sphere.radius,sphereCenter:c.sphere.center.toArray()})),o.maxInstanceCount=this._maxInstanceCount,o.maxVertexCount=this._maxVertexCount,o.maxIndexCount=this._maxIndexCount,o.geometryInitialized=this._geometryInitialized,o.geometryCount=this._geometryCount,o.matricesTexture=this._matricesTexture.toJSON(e),this._colorsTexture!==null&&(o.colorsTexture=this._colorsTexture.toJSON(e)),this.boundingSphere!==null&&(o.boundingSphere={center:o.boundingSphere.center.toArray(),radius:o.boundingSphere.radius}),this.boundingBox!==null&&(o.boundingBox={min:o.boundingBox.min.toArray(),max:o.boundingBox.max.toArray()}));function a(c,h){return c[h.uuid]===void 0&&(c[h.uuid]=h.toJSON(e)),h.uuid}if(this.isScene)this.background&&(this.background.isColor?o.background=this.background.toJSON():this.background.isTexture&&(o.background=this.background.toJSON(e).uuid)),this.environment&&this.environment.isTexture&&this.environment.isRenderTargetTexture!==!0&&(o.environment=this.environment.toJSON(e).uuid);else if(this.isMesh||this.isLine||this.isPoints){o.geometry=a(e.geometries,this.geometry);const c=this.geometry.parameters;if(c!==void 0&&c.shapes!==void 0){const h=c.shapes;if(Array.isArray(h))for(let d=0,m=h.length;d<m;d++){const _=h[d];a(e.shapes,_)}else a(e.shapes,h)}}if(this.isSkinnedMesh&&(o.bindMode=this.bindMode,o.bindMatrix=this.bindMatrix.toArray(),this.skeleton!==void 0&&(a(e.skeletons,this.skeleton),o.skeleton=this.skeleton.uuid)),this.material!==void 0)if(Array.isArray(this.material)){const c=[];for(let h=0,d=this.material.length;h<d;h++)c.push(a(e.materials,this.material[h]));o.material=c}else o.material=a(e.materials,this.material);if(this.children.length>0){o.children=[];for(let c=0;c<this.children.length;c++)o.children.push(this.children[c].toJSON(e).object)}if(this.animations.length>0){o.animations=[];for(let c=0;c<this.animations.length;c++){const h=this.animations[c];o.animations.push(a(e.animations,h))}}if(t){const c=u(e.geometries),h=u(e.materials),d=u(e.textures),m=u(e.images),_=u(e.shapes),g=u(e.skeletons),S=u(e.animations),M=u(e.nodes);c.length>0&&(i.geometries=c),h.length>0&&(i.materials=h),d.length>0&&(i.textures=d),m.length>0&&(i.images=m),_.length>0&&(i.shapes=_),g.length>0&&(i.skeletons=g),S.length>0&&(i.animations=S),M.length>0&&(i.nodes=M)}return i.object=o,i;function u(c){const h=[];for(const d in c){const m=c[d];delete m.metadata,h.push(m)}return h}}clone(e){return new this.constructor().copy(this,e)}copy(e,t=!0){if(this.name=e.name,this.up.copy(e.up),this.position.copy(e.position),this.rotation.order=e.rotation.order,this.quaternion.copy(e.quaternion),this.scale.copy(e.scale),this.matrix.copy(e.matrix),this.matrixWorld.copy(e.matrixWorld),this.matrixAutoUpdate=e.matrixAutoUpdate,this.matrixWorldAutoUpdate=e.matrixWorldAutoUpdate,this.matrixWorldNeedsUpdate=e.matrixWorldNeedsUpdate,this.layers.mask=e.layers.mask,this.visible=e.visible,this.castShadow=e.castShadow,this.receiveShadow=e.receiveShadow,this.frustumCulled=e.frustumCulled,this.renderOrder=e.renderOrder,this.animations=e.animations.slice(),this.userData=JSON.parse(JSON.stringify(e.userData)),t===!0)for(let i=0;i<e.children.length;i++){const o=e.children[i];this.add(o.clone())}return this}}cn.DEFAULT_UP=new W(0,1,0);cn.DEFAULT_MATRIX_AUTO_UPDATE=!0;cn.DEFAULT_MATRIX_WORLD_AUTO_UPDATE=!0;const pi=new W,Vi=new W,ih=new W,Gi=new W,qs=new W,Ys=new W,i0=new W,rh=new W,sh=new W,oh=new W;class ii{constructor(e=new W,t=new W,i=new W){this.a=e,this.b=t,this.c=i}static getNormal(e,t,i,o){o.subVectors(i,t),pi.subVectors(e,t),o.cross(pi);const a=o.lengthSq();return a>0?o.multiplyScalar(1/Math.sqrt(a)):o.set(0,0,0)}static getBarycoord(e,t,i,o,a){pi.subVectors(o,t),Vi.subVectors(i,t),ih.subVectors(e,t);const u=pi.dot(pi),c=pi.dot(Vi),h=pi.dot(ih),d=Vi.dot(Vi),m=Vi.dot(ih),_=u*d-c*c;if(_===0)return a.set(0,0,0),null;const g=1/_,S=(d*h-c*m)*g,M=(u*m-c*h)*g;return a.set(1-S-M,M,S)}static containsPoint(e,t,i,o){return this.getBarycoord(e,t,i,o,Gi)===null?!1:Gi.x>=0&&Gi.y>=0&&Gi.x+Gi.y<=1}static getInterpolation(e,t,i,o,a,u,c,h){return this.getBarycoord(e,t,i,o,Gi)===null?(h.x=0,h.y=0,"z"in h&&(h.z=0),"w"in h&&(h.w=0),null):(h.setScalar(0),h.addScaledVector(a,Gi.x),h.addScaledVector(u,Gi.y),h.addScaledVector(c,Gi.z),h)}static isFrontFacing(e,t,i,o){return pi.subVectors(i,t),Vi.subVectors(e,t),pi.cross(Vi).dot(o)<0}set(e,t,i){return this.a.copy(e),this.b.copy(t),this.c.copy(i),this}setFromPointsAndIndices(e,t,i,o){return this.a.copy(e[t]),this.b.copy(e[i]),this.c.copy(e[o]),this}setFromAttributeAndIndices(e,t,i,o){return this.a.fromBufferAttribute(e,t),this.b.fromBufferAttribute(e,i),this.c.fromBufferAttribute(e,o),this}clone(){return new this.constructor().copy(this)}copy(e){return this.a.copy(e.a),this.b.copy(e.b),this.c.copy(e.c),this}getArea(){return pi.subVectors(this.c,this.b),Vi.subVectors(this.a,this.b),pi.cross(Vi).length()*.5}getMidpoint(e){return e.addVectors(this.a,this.b).add(this.c).multiplyScalar(1/3)}getNormal(e){return ii.getNormal(this.a,this.b,this.c,e)}getPlane(e){return e.setFromCoplanarPoints(this.a,this.b,this.c)}getBarycoord(e,t){return ii.getBarycoord(e,this.a,this.b,this.c,t)}getInterpolation(e,t,i,o,a){return ii.getInterpolation(e,this.a,this.b,this.c,t,i,o,a)}containsPoint(e){return ii.containsPoint(e,this.a,this.b,this.c)}isFrontFacing(e){return ii.isFrontFacing(this.a,this.b,this.c,e)}intersectsBox(e){return e.intersectsTriangle(this)}closestPointToPoint(e,t){const i=this.a,o=this.b,a=this.c;let u,c;qs.subVectors(o,i),Ys.subVectors(a,i),rh.subVectors(e,i);const h=qs.dot(rh),d=Ys.dot(rh);if(h<=0&&d<=0)return t.copy(i);sh.subVectors(e,o);const m=qs.dot(sh),_=Ys.dot(sh);if(m>=0&&_<=m)return t.copy(o);const g=h*_-m*d;if(g<=0&&h>=0&&m<=0)return u=h/(h-m),t.copy(i).addScaledVector(qs,u);oh.subVectors(e,a);const S=qs.dot(oh),M=Ys.dot(oh);if(M>=0&&S<=M)return t.copy(a);const E=S*d-h*M;if(E<=0&&d>=0&&M<=0)return c=d/(d-M),t.copy(i).addScaledVector(Ys,c);const y=m*M-S*_;if(y<=0&&_-m>=0&&S-M>=0)return i0.subVectors(a,o),c=(_-m)/(_-m+(S-M)),t.copy(o).addScaledVector(i0,c);const x=1/(y+E+g);return u=E*x,c=g*x,t.copy(i).addScaledVector(qs,u).addScaledVector(Ys,c)}equals(e){return e.a.equals(this.a)&&e.b.equals(this.b)&&e.c.equals(this.c)}}const C_={aliceblue:15792383,antiquewhite:16444375,aqua:65535,aquamarine:8388564,azure:15794175,beige:16119260,bisque:16770244,black:0,blanchedalmond:16772045,blue:255,blueviolet:9055202,brown:10824234,burlywood:14596231,cadetblue:6266528,chartreuse:8388352,chocolate:13789470,coral:16744272,cornflowerblue:6591981,cornsilk:16775388,crimson:14423100,cyan:65535,darkblue:139,darkcyan:35723,darkgoldenrod:12092939,darkgray:11119017,darkgreen:25600,darkgrey:11119017,darkkhaki:12433259,darkmagenta:9109643,darkolivegreen:5597999,darkorange:16747520,darkorchid:10040012,darkred:9109504,darksalmon:15308410,darkseagreen:9419919,darkslateblue:4734347,darkslategray:3100495,darkslategrey:3100495,darkturquoise:52945,darkviolet:9699539,deeppink:16716947,deepskyblue:49151,dimgray:6908265,dimgrey:6908265,dodgerblue:2003199,firebrick:11674146,floralwhite:16775920,forestgreen:2263842,fuchsia:16711935,gainsboro:14474460,ghostwhite:16316671,gold:16766720,goldenrod:14329120,gray:8421504,green:32768,greenyellow:11403055,grey:8421504,honeydew:15794160,hotpink:16738740,indianred:13458524,indigo:4915330,ivory:16777200,khaki:15787660,lavender:15132410,lavenderblush:16773365,lawngreen:8190976,lemonchiffon:16775885,lightblue:11393254,lightcoral:15761536,lightcyan:14745599,lightgoldenrodyellow:16448210,lightgray:13882323,lightgreen:9498256,lightgrey:13882323,lightpink:16758465,lightsalmon:16752762,lightseagreen:2142890,lightskyblue:8900346,lightslategray:7833753,lightslategrey:7833753,lightsteelblue:11584734,lightyellow:16777184,lime:65280,limegreen:3329330,linen:16445670,magenta:16711935,maroon:8388608,mediumaquamarine:6737322,mediumblue:205,mediumorchid:12211667,mediumpurple:9662683,mediumseagreen:3978097,mediumslateblue:8087790,mediumspringgreen:64154,mediumturquoise:4772300,mediumvioletred:13047173,midnightblue:1644912,mintcream:16121850,mistyrose:16770273,moccasin:16770229,navajowhite:16768685,navy:128,oldlace:16643558,olive:8421376,olivedrab:7048739,orange:16753920,orangered:16729344,orchid:14315734,palegoldenrod:15657130,palegreen:10025880,paleturquoise:11529966,palevioletred:14381203,papayawhip:16773077,peachpuff:16767673,peru:13468991,pink:16761035,plum:14524637,powderblue:11591910,purple:8388736,rebeccapurple:6697881,red:16711680,rosybrown:12357519,royalblue:4286945,saddlebrown:9127187,salmon:16416882,sandybrown:16032864,seagreen:3050327,seashell:16774638,sienna:10506797,silver:12632256,skyblue:8900331,slateblue:6970061,slategray:7372944,slategrey:7372944,snow:16775930,springgreen:65407,steelblue:4620980,tan:13808780,teal:32896,thistle:14204888,tomato:16737095,turquoise:4251856,violet:15631086,wheat:16113331,white:16777215,whitesmoke:16119285,yellow:16776960,yellowgreen:10145074},Tr={h:0,s:0,l:0},Wl={h:0,s:0,l:0};function ah(s,e,t){return t<0&&(t+=1),t>1&&(t-=1),t<1/6?s+(e-s)*6*t:t<1/2?e:t<2/3?s+(e-s)*6*(2/3-t):s}class yt{constructor(e,t,i){return this.isColor=!0,this.r=1,this.g=1,this.b=1,this.set(e,t,i)}set(e,t,i){if(t===void 0&&i===void 0){const o=e;o&&o.isColor?this.copy(o):typeof o=="number"?this.setHex(o):typeof o=="string"&&this.setStyle(o)}else this.setRGB(e,t,i);return this}setScalar(e){return this.r=e,this.g=e,this.b=e,this}setHex(e,t=Mi){return e=Math.floor(e),this.r=(e>>16&255)/255,this.g=(e>>8&255)/255,this.b=(e&255)/255,Tt.toWorkingColorSpace(this,t),this}setRGB(e,t,i,o=Tt.workingColorSpace){return this.r=e,this.g=t,this.b=i,Tt.toWorkingColorSpace(this,o),this}setHSL(e,t,i,o=Tt.workingColorSpace){if(e=_S(e,1),t=pn(t,0,1),i=pn(i,0,1),t===0)this.r=this.g=this.b=i;else{const a=i<=.5?i*(1+t):i+t-i*t,u=2*i-a;this.r=ah(u,a,e+1/3),this.g=ah(u,a,e),this.b=ah(u,a,e-1/3)}return Tt.toWorkingColorSpace(this,o),this}setStyle(e,t=Mi){function i(a){a!==void 0&&parseFloat(a)<1&&console.warn("THREE.Color: Alpha component of "+e+" will be ignored.")}let o;if(o=/^(\w+)\(([^\)]*)\)/.exec(e)){let a;const u=o[1],c=o[2];switch(u){case"rgb":case"rgba":if(a=/^\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*(?:,\s*(\d*\.?\d+)\s*)?$/.exec(c))return i(a[4]),this.setRGB(Math.min(255,parseInt(a[1],10))/255,Math.min(255,parseInt(a[2],10))/255,Math.min(255,parseInt(a[3],10))/255,t);if(a=/^\s*(\d+)\%\s*,\s*(\d+)\%\s*,\s*(\d+)\%\s*(?:,\s*(\d*\.?\d+)\s*)?$/.exec(c))return i(a[4]),this.setRGB(Math.min(100,parseInt(a[1],10))/100,Math.min(100,parseInt(a[2],10))/100,Math.min(100,parseInt(a[3],10))/100,t);break;case"hsl":case"hsla":if(a=/^\s*(\d*\.?\d+)\s*,\s*(\d*\.?\d+)\%\s*,\s*(\d*\.?\d+)\%\s*(?:,\s*(\d*\.?\d+)\s*)?$/.exec(c))return i(a[4]),this.setHSL(parseFloat(a[1])/360,parseFloat(a[2])/100,parseFloat(a[3])/100,t);break;default:console.warn("THREE.Color: Unknown color model "+e)}}else if(o=/^\#([A-Fa-f\d]+)$/.exec(e)){const a=o[1],u=a.length;if(u===3)return this.setRGB(parseInt(a.charAt(0),16)/15,parseInt(a.charAt(1),16)/15,parseInt(a.charAt(2),16)/15,t);if(u===6)return this.setHex(parseInt(a,16),t);console.warn("THREE.Color: Invalid hex color "+e)}else if(e&&e.length>0)return this.setColorName(e,t);return this}setColorName(e,t=Mi){const i=C_[e.toLowerCase()];return i!==void 0?this.setHex(i,t):console.warn("THREE.Color: Unknown color "+e),this}clone(){return new this.constructor(this.r,this.g,this.b)}copy(e){return this.r=e.r,this.g=e.g,this.b=e.b,this}copySRGBToLinear(e){return this.r=_o(e.r),this.g=_o(e.g),this.b=_o(e.b),this}copyLinearToSRGB(e){return this.r=Yf(e.r),this.g=Yf(e.g),this.b=Yf(e.b),this}convertSRGBToLinear(){return this.copySRGBToLinear(this),this}convertLinearToSRGB(){return this.copyLinearToSRGB(this),this}getHex(e=Mi){return Tt.fromWorkingColorSpace(Mn.copy(this),e),Math.round(pn(Mn.r*255,0,255))*65536+Math.round(pn(Mn.g*255,0,255))*256+Math.round(pn(Mn.b*255,0,255))}getHexString(e=Mi){return("000000"+this.getHex(e).toString(16)).slice(-6)}getHSL(e,t=Tt.workingColorSpace){Tt.fromWorkingColorSpace(Mn.copy(this),t);const i=Mn.r,o=Mn.g,a=Mn.b,u=Math.max(i,o,a),c=Math.min(i,o,a);let h,d;const m=(c+u)/2;if(c===u)h=0,d=0;else{const _=u-c;switch(d=m<=.5?_/(u+c):_/(2-u-c),u){case i:h=(o-a)/_+(o<a?6:0);break;case o:h=(a-i)/_+2;break;case a:h=(i-o)/_+4;break}h/=6}return e.h=h,e.s=d,e.l=m,e}getRGB(e,t=Tt.workingColorSpace){return Tt.fromWorkingColorSpace(Mn.copy(this),t),e.r=Mn.r,e.g=Mn.g,e.b=Mn.b,e}getStyle(e=Mi){Tt.fromWorkingColorSpace(Mn.copy(this),e);const t=Mn.r,i=Mn.g,o=Mn.b;return e!==Mi?`color(${e} ${t.toFixed(3)} ${i.toFixed(3)} ${o.toFixed(3)})`:`rgb(${Math.round(t*255)},${Math.round(i*255)},${Math.round(o*255)})`}offsetHSL(e,t,i){return this.getHSL(Tr),this.setHSL(Tr.h+e,Tr.s+t,Tr.l+i)}add(e){return this.r+=e.r,this.g+=e.g,this.b+=e.b,this}addColors(e,t){return this.r=e.r+t.r,this.g=e.g+t.g,this.b=e.b+t.b,this}addScalar(e){return this.r+=e,this.g+=e,this.b+=e,this}sub(e){return this.r=Math.max(0,this.r-e.r),this.g=Math.max(0,this.g-e.g),this.b=Math.max(0,this.b-e.b),this}multiply(e){return this.r*=e.r,this.g*=e.g,this.b*=e.b,this}multiplyScalar(e){return this.r*=e,this.g*=e,this.b*=e,this}lerp(e,t){return this.r+=(e.r-this.r)*t,this.g+=(e.g-this.g)*t,this.b+=(e.b-this.b)*t,this}lerpColors(e,t,i){return this.r=e.r+(t.r-e.r)*i,this.g=e.g+(t.g-e.g)*i,this.b=e.b+(t.b-e.b)*i,this}lerpHSL(e,t){this.getHSL(Tr),e.getHSL(Wl);const i=jf(Tr.h,Wl.h,t),o=jf(Tr.s,Wl.s,t),a=jf(Tr.l,Wl.l,t);return this.setHSL(i,o,a),this}setFromVector3(e){return this.r=e.x,this.g=e.y,this.b=e.z,this}applyMatrix3(e){const t=this.r,i=this.g,o=this.b,a=e.elements;return this.r=a[0]*t+a[3]*i+a[6]*o,this.g=a[1]*t+a[4]*i+a[7]*o,this.b=a[2]*t+a[5]*i+a[8]*o,this}equals(e){return e.r===this.r&&e.g===this.g&&e.b===this.b}fromArray(e,t=0){return this.r=e[t],this.g=e[t+1],this.b=e[t+2],this}toArray(e=[],t=0){return e[t]=this.r,e[t+1]=this.g,e[t+2]=this.b,e}fromBufferAttribute(e,t){return this.r=e.getX(t),this.g=e.getY(t),this.b=e.getZ(t),this}toJSON(){return this.getHex()}*[Symbol.iterator](){yield this.r,yield this.g,yield this.b}}const Mn=new yt;yt.NAMES=C_;let IS=0;class ds extends hs{constructor(){super(),this.isMaterial=!0,Object.defineProperty(this,"id",{value:IS++}),this.uuid=Yi(),this.name="",this.type="Material",this.blending=po,this.side=Dr,this.vertexColors=!1,this.opacity=1,this.transparent=!1,this.alphaHash=!1,this.blendSrc=Ah,this.blendDst=Ch,this.blendEquation=ns,this.blendSrcAlpha=null,this.blendDstAlpha=null,this.blendEquationAlpha=null,this.blendColor=new yt(0,0,0),this.blendAlpha=0,this.depthFunc=Eu,this.depthTest=!0,this.depthWrite=!0,this.stencilWriteMask=255,this.stencilFunc=Wg,this.stencilRef=0,this.stencilFuncMask=255,this.stencilFail=zs,this.stencilZFail=zs,this.stencilZPass=zs,this.stencilWrite=!1,this.clippingPlanes=null,this.clipIntersection=!1,this.clipShadows=!1,this.shadowSide=null,this.colorWrite=!0,this.precision=null,this.polygonOffset=!1,this.polygonOffsetFactor=0,this.polygonOffsetUnits=0,this.dithering=!1,this.alphaToCoverage=!1,this.premultipliedAlpha=!1,this.forceSinglePass=!1,this.visible=!0,this.toneMapped=!0,this.userData={},this.version=0,this._alphaTest=0}get alphaTest(){return this._alphaTest}set alphaTest(e){this._alphaTest>0!=e>0&&this.version++,this._alphaTest=e}onBeforeRender(){}onBeforeCompile(){}customProgramCacheKey(){return this.onBeforeCompile.toString()}setValues(e){if(e!==void 0)for(const t in e){const i=e[t];if(i===void 0){console.warn(`THREE.Material: parameter '${t}' has value of undefined.`);continue}const o=this[t];if(o===void 0){console.warn(`THREE.Material: '${t}' is not a property of THREE.${this.type}.`);continue}o&&o.isColor?o.set(i):o&&o.isVector3&&i&&i.isVector3?o.copy(i):this[t]=i}}toJSON(e){const t=e===void 0||typeof e=="string";t&&(e={textures:{},images:{}});const i={metadata:{version:4.6,type:"Material",generator:"Material.toJSON"}};i.uuid=this.uuid,i.type=this.type,this.name!==""&&(i.name=this.name),this.color&&this.color.isColor&&(i.color=this.color.getHex()),this.roughness!==void 0&&(i.roughness=this.roughness),this.metalness!==void 0&&(i.metalness=this.metalness),this.sheen!==void 0&&(i.sheen=this.sheen),this.sheenColor&&this.sheenColor.isColor&&(i.sheenColor=this.sheenColor.getHex()),this.sheenRoughness!==void 0&&(i.sheenRoughness=this.sheenRoughness),this.emissive&&this.emissive.isColor&&(i.emissive=this.emissive.getHex()),this.emissiveIntensity!==void 0&&this.emissiveIntensity!==1&&(i.emissiveIntensity=this.emissiveIntensity),this.specular&&this.specular.isColor&&(i.specular=this.specular.getHex()),this.specularIntensity!==void 0&&(i.specularIntensity=this.specularIntensity),this.specularColor&&this.specularColor.isColor&&(i.specularColor=this.specularColor.getHex()),this.shininess!==void 0&&(i.shininess=this.shininess),this.clearcoat!==void 0&&(i.clearcoat=this.clearcoat),this.clearcoatRoughness!==void 0&&(i.clearcoatRoughness=this.clearcoatRoughness),this.clearcoatMap&&this.clearcoatMap.isTexture&&(i.clearcoatMap=this.clearcoatMap.toJSON(e).uuid),this.clearcoatRoughnessMap&&this.clearcoatRoughnessMap.isTexture&&(i.clearcoatRoughnessMap=this.clearcoatRoughnessMap.toJSON(e).uuid),this.clearcoatNormalMap&&this.clearcoatNormalMap.isTexture&&(i.clearcoatNormalMap=this.clearcoatNormalMap.toJSON(e).uuid,i.clearcoatNormalScale=this.clearcoatNormalScale.toArray()),this.dispersion!==void 0&&(i.dispersion=this.dispersion),this.iridescence!==void 0&&(i.iridescence=this.iridescence),this.iridescenceIOR!==void 0&&(i.iridescenceIOR=this.iridescenceIOR),this.iridescenceThicknessRange!==void 0&&(i.iridescenceThicknessRange=this.iridescenceThicknessRange),this.iridescenceMap&&this.iridescenceMap.isTexture&&(i.iridescenceMap=this.iridescenceMap.toJSON(e).uuid),this.iridescenceThicknessMap&&this.iridescenceThicknessMap.isTexture&&(i.iridescenceThicknessMap=this.iridescenceThicknessMap.toJSON(e).uuid),this.anisotropy!==void 0&&(i.anisotropy=this.anisotropy),this.anisotropyRotation!==void 0&&(i.anisotropyRotation=this.anisotropyRotation),this.anisotropyMap&&this.anisotropyMap.isTexture&&(i.anisotropyMap=this.anisotropyMap.toJSON(e).uuid),this.map&&this.map.isTexture&&(i.map=this.map.toJSON(e).uuid),this.matcap&&this.matcap.isTexture&&(i.matcap=this.matcap.toJSON(e).uuid),this.alphaMap&&this.alphaMap.isTexture&&(i.alphaMap=this.alphaMap.toJSON(e).uuid),this.lightMap&&this.lightMap.isTexture&&(i.lightMap=this.lightMap.toJSON(e).uuid,i.lightMapIntensity=this.lightMapIntensity),this.aoMap&&this.aoMap.isTexture&&(i.aoMap=this.aoMap.toJSON(e).uuid,i.aoMapIntensity=this.aoMapIntensity),this.bumpMap&&this.bumpMap.isTexture&&(i.bumpMap=this.bumpMap.toJSON(e).uuid,i.bumpScale=this.bumpScale),this.normalMap&&this.normalMap.isTexture&&(i.normalMap=this.normalMap.toJSON(e).uuid,i.normalMapType=this.normalMapType,i.normalScale=this.normalScale.toArray()),this.displacementMap&&this.displacementMap.isTexture&&(i.displacementMap=this.displacementMap.toJSON(e).uuid,i.displacementScale=this.displacementScale,i.displacementBias=this.displacementBias),this.roughnessMap&&this.roughnessMap.isTexture&&(i.roughnessMap=this.roughnessMap.toJSON(e).uuid),this.metalnessMap&&this.metalnessMap.isTexture&&(i.metalnessMap=this.metalnessMap.toJSON(e).uuid),this.emissiveMap&&this.emissiveMap.isTexture&&(i.emissiveMap=this.emissiveMap.toJSON(e).uuid),this.specularMap&&this.specularMap.isTexture&&(i.specularMap=this.specularMap.toJSON(e).uuid),this.specularIntensityMap&&this.specularIntensityMap.isTexture&&(i.specularIntensityMap=this.specularIntensityMap.toJSON(e).uuid),this.specularColorMap&&this.specularColorMap.isTexture&&(i.specularColorMap=this.specularColorMap.toJSON(e).uuid),this.envMap&&this.envMap.isTexture&&(i.envMap=this.envMap.toJSON(e).uuid,this.combine!==void 0&&(i.combine=this.combine)),this.envMapRotation!==void 0&&(i.envMapRotation=this.envMapRotation.toArray()),this.envMapIntensity!==void 0&&(i.envMapIntensity=this.envMapIntensity),this.reflectivity!==void 0&&(i.reflectivity=this.reflectivity),this.refractionRatio!==void 0&&(i.refractionRatio=this.refractionRatio),this.gradientMap&&this.gradientMap.isTexture&&(i.gradientMap=this.gradientMap.toJSON(e).uuid),this.transmission!==void 0&&(i.transmission=this.transmission),this.transmissionMap&&this.transmissionMap.isTexture&&(i.transmissionMap=this.transmissionMap.toJSON(e).uuid),this.thickness!==void 0&&(i.thickness=this.thickness),this.thicknessMap&&this.thicknessMap.isTexture&&(i.thicknessMap=this.thicknessMap.toJSON(e).uuid),this.attenuationDistance!==void 0&&this.attenuationDistance!==1/0&&(i.attenuationDistance=this.attenuationDistance),this.attenuationColor!==void 0&&(i.attenuationColor=this.attenuationColor.getHex()),this.size!==void 0&&(i.size=this.size),this.shadowSide!==null&&(i.shadowSide=this.shadowSide),this.sizeAttenuation!==void 0&&(i.sizeAttenuation=this.sizeAttenuation),this.blending!==po&&(i.blending=this.blending),this.side!==Dr&&(i.side=this.side),this.vertexColors===!0&&(i.vertexColors=!0),this.opacity<1&&(i.opacity=this.opacity),this.transparent===!0&&(i.transparent=!0),this.blendSrc!==Ah&&(i.blendSrc=this.blendSrc),this.blendDst!==Ch&&(i.blendDst=this.blendDst),this.blendEquation!==ns&&(i.blendEquation=this.blendEquation),this.blendSrcAlpha!==null&&(i.blendSrcAlpha=this.blendSrcAlpha),this.blendDstAlpha!==null&&(i.blendDstAlpha=this.blendDstAlpha),this.blendEquationAlpha!==null&&(i.blendEquationAlpha=this.blendEquationAlpha),this.blendColor&&this.blendColor.isColor&&(i.blendColor=this.blendColor.getHex()),this.blendAlpha!==0&&(i.blendAlpha=this.blendAlpha),this.depthFunc!==Eu&&(i.depthFunc=this.depthFunc),this.depthTest===!1&&(i.depthTest=this.depthTest),this.depthWrite===!1&&(i.depthWrite=this.depthWrite),this.colorWrite===!1&&(i.colorWrite=this.colorWrite),this.stencilWriteMask!==255&&(i.stencilWriteMask=this.stencilWriteMask),this.stencilFunc!==Wg&&(i.stencilFunc=this.stencilFunc),this.stencilRef!==0&&(i.stencilRef=this.stencilRef),this.stencilFuncMask!==255&&(i.stencilFuncMask=this.stencilFuncMask),this.stencilFail!==zs&&(i.stencilFail=this.stencilFail),this.stencilZFail!==zs&&(i.stencilZFail=this.stencilZFail),this.stencilZPass!==zs&&(i.stencilZPass=this.stencilZPass),this.stencilWrite===!0&&(i.stencilWrite=this.stencilWrite),this.rotation!==void 0&&this.rotation!==0&&(i.rotation=this.rotation),this.polygonOffset===!0&&(i.polygonOffset=!0),this.polygonOffsetFactor!==0&&(i.polygonOffsetFactor=this.polygonOffsetFactor),this.polygonOffsetUnits!==0&&(i.polygonOffsetUnits=this.polygonOffsetUnits),this.linewidth!==void 0&&this.linewidth!==1&&(i.linewidth=this.linewidth),this.dashSize!==void 0&&(i.dashSize=this.dashSize),this.gapSize!==void 0&&(i.gapSize=this.gapSize),this.scale!==void 0&&(i.scale=this.scale),this.dithering===!0&&(i.dithering=!0),this.alphaTest>0&&(i.alphaTest=this.alphaTest),this.alphaHash===!0&&(i.alphaHash=!0),this.alphaToCoverage===!0&&(i.alphaToCoverage=!0),this.premultipliedAlpha===!0&&(i.premultipliedAlpha=!0),this.forceSinglePass===!0&&(i.forceSinglePass=!0),this.wireframe===!0&&(i.wireframe=!0),this.wireframeLinewidth>1&&(i.wireframeLinewidth=this.wireframeLinewidth),this.wireframeLinecap!=="round"&&(i.wireframeLinecap=this.wireframeLinecap),this.wireframeLinejoin!=="round"&&(i.wireframeLinejoin=this.wireframeLinejoin),this.flatShading===!0&&(i.flatShading=!0),this.visible===!1&&(i.visible=!1),this.toneMapped===!1&&(i.toneMapped=!1),this.fog===!1&&(i.fog=!1),Object.keys(this.userData).length>0&&(i.userData=this.userData);function o(a){const u=[];for(const c in a){const h=a[c];delete h.metadata,u.push(h)}return u}if(t){const a=o(e.textures),u=o(e.images);a.length>0&&(i.textures=a),u.length>0&&(i.images=u)}return i}clone(){return new this.constructor().copy(this)}copy(e){this.name=e.name,this.blending=e.blending,this.side=e.side,this.vertexColors=e.vertexColors,this.opacity=e.opacity,this.transparent=e.transparent,this.blendSrc=e.blendSrc,this.blendDst=e.blendDst,this.blendEquation=e.blendEquation,this.blendSrcAlpha=e.blendSrcAlpha,this.blendDstAlpha=e.blendDstAlpha,this.blendEquationAlpha=e.blendEquationAlpha,this.blendColor.copy(e.blendColor),this.blendAlpha=e.blendAlpha,this.depthFunc=e.depthFunc,this.depthTest=e.depthTest,this.depthWrite=e.depthWrite,this.stencilWriteMask=e.stencilWriteMask,this.stencilFunc=e.stencilFunc,this.stencilRef=e.stencilRef,this.stencilFuncMask=e.stencilFuncMask,this.stencilFail=e.stencilFail,this.stencilZFail=e.stencilZFail,this.stencilZPass=e.stencilZPass,this.stencilWrite=e.stencilWrite;const t=e.clippingPlanes;let i=null;if(t!==null){const o=t.length;i=new Array(o);for(let a=0;a!==o;++a)i[a]=t[a].clone()}return this.clippingPlanes=i,this.clipIntersection=e.clipIntersection,this.clipShadows=e.clipShadows,this.shadowSide=e.shadowSide,this.colorWrite=e.colorWrite,this.precision=e.precision,this.polygonOffset=e.polygonOffset,this.polygonOffsetFactor=e.polygonOffsetFactor,this.polygonOffsetUnits=e.polygonOffsetUnits,this.dithering=e.dithering,this.alphaTest=e.alphaTest,this.alphaHash=e.alphaHash,this.alphaToCoverage=e.alphaToCoverage,this.premultipliedAlpha=e.premultipliedAlpha,this.forceSinglePass=e.forceSinglePass,this.visible=e.visible,this.toneMapped=e.toneMapped,this.userData=JSON.parse(JSON.stringify(e.userData)),this}dispose(){this.dispatchEvent({type:"dispose"})}set needsUpdate(e){e===!0&&this.version++}onBuild(){console.warn("Material: onBuild() has been removed.")}}class R_ extends ds{constructor(e){super(),this.isMeshBasicMaterial=!0,this.type="MeshBasicMaterial",this.color=new yt(16777215),this.map=null,this.lightMap=null,this.lightMapIntensity=1,this.aoMap=null,this.aoMapIntensity=1,this.specularMap=null,this.alphaMap=null,this.envMap=null,this.envMapRotation=new Ai,this.combine=u_,this.reflectivity=1,this.refractionRatio=.98,this.wireframe=!1,this.wireframeLinewidth=1,this.wireframeLinecap="round",this.wireframeLinejoin="round",this.fog=!0,this.setValues(e)}copy(e){return super.copy(e),this.color.copy(e.color),this.map=e.map,this.lightMap=e.lightMap,this.lightMapIntensity=e.lightMapIntensity,this.aoMap=e.aoMap,this.aoMapIntensity=e.aoMapIntensity,this.specularMap=e.specularMap,this.alphaMap=e.alphaMap,this.envMap=e.envMap,this.envMapRotation.copy(e.envMapRotation),this.combine=e.combine,this.reflectivity=e.reflectivity,this.refractionRatio=e.refractionRatio,this.wireframe=e.wireframe,this.wireframeLinewidth=e.wireframeLinewidth,this.wireframeLinecap=e.wireframeLinecap,this.wireframeLinejoin=e.wireframeLinejoin,this.fog=e.fog,this}}const Kt=new W,Xl=new Pe;class gi{constructor(e,t,i=!1){if(Array.isArray(e))throw new TypeError("THREE.BufferAttribute: array should be a Typed Array.");this.isBufferAttribute=!0,this.name="",this.array=e,this.itemSize=t,this.count=e!==void 0?e.length/t:0,this.normalized=i,this.usage=rd,this._updateRange={offset:0,count:-1},this.updateRanges=[],this.gpuType=Xi,this.version=0}onUploadCallback(){}set needsUpdate(e){e===!0&&this.version++}get updateRange(){return go("THREE.BufferAttribute: updateRange() is deprecated and will be removed in r169. Use addUpdateRange() instead."),this._updateRange}setUsage(e){return this.usage=e,this}addUpdateRange(e,t){this.updateRanges.push({start:e,count:t})}clearUpdateRanges(){this.updateRanges.length=0}copy(e){return this.name=e.name,this.array=new e.array.constructor(e.array),this.itemSize=e.itemSize,this.count=e.count,this.normalized=e.normalized,this.usage=e.usage,this.gpuType=e.gpuType,this}copyAt(e,t,i){e*=this.itemSize,i*=t.itemSize;for(let o=0,a=this.itemSize;o<a;o++)this.array[e+o]=t.array[i+o];return this}copyArray(e){return this.array.set(e),this}applyMatrix3(e){if(this.itemSize===2)for(let t=0,i=this.count;t<i;t++)Xl.fromBufferAttribute(this,t),Xl.applyMatrix3(e),this.setXY(t,Xl.x,Xl.y);else if(this.itemSize===3)for(let t=0,i=this.count;t<i;t++)Kt.fromBufferAttribute(this,t),Kt.applyMatrix3(e),this.setXYZ(t,Kt.x,Kt.y,Kt.z);return this}applyMatrix4(e){for(let t=0,i=this.count;t<i;t++)Kt.fromBufferAttribute(this,t),Kt.applyMatrix4(e),this.setXYZ(t,Kt.x,Kt.y,Kt.z);return this}applyNormalMatrix(e){for(let t=0,i=this.count;t<i;t++)Kt.fromBufferAttribute(this,t),Kt.applyNormalMatrix(e),this.setXYZ(t,Kt.x,Kt.y,Kt.z);return this}transformDirection(e){for(let t=0,i=this.count;t<i;t++)Kt.fromBufferAttribute(this,t),Kt.transformDirection(e),this.setXYZ(t,Kt.x,Kt.y,Kt.z);return this}set(e,t=0){return this.array.set(e,t),this}getComponent(e,t){let i=this.array[e*this.itemSize+t];return this.normalized&&(i=Ti(i,this.array)),i}setComponent(e,t,i){return this.normalized&&(i=Rt(i,this.array)),this.array[e*this.itemSize+t]=i,this}getX(e){let t=this.array[e*this.itemSize];return this.normalized&&(t=Ti(t,this.array)),t}setX(e,t){return this.normalized&&(t=Rt(t,this.array)),this.array[e*this.itemSize]=t,this}getY(e){let t=this.array[e*this.itemSize+1];return this.normalized&&(t=Ti(t,this.array)),t}setY(e,t){return this.normalized&&(t=Rt(t,this.array)),this.array[e*this.itemSize+1]=t,this}getZ(e){let t=this.array[e*this.itemSize+2];return this.normalized&&(t=Ti(t,this.array)),t}setZ(e,t){return this.normalized&&(t=Rt(t,this.array)),this.array[e*this.itemSize+2]=t,this}getW(e){let t=this.array[e*this.itemSize+3];return this.normalized&&(t=Ti(t,this.array)),t}setW(e,t){return this.normalized&&(t=Rt(t,this.array)),this.array[e*this.itemSize+3]=t,this}setXY(e,t,i){return e*=this.itemSize,this.normalized&&(t=Rt(t,this.array),i=Rt(i,this.array)),this.array[e+0]=t,this.array[e+1]=i,this}setXYZ(e,t,i,o){return e*=this.itemSize,this.normalized&&(t=Rt(t,this.array),i=Rt(i,this.array),o=Rt(o,this.array)),this.array[e+0]=t,this.array[e+1]=i,this.array[e+2]=o,this}setXYZW(e,t,i,o,a){return e*=this.itemSize,this.normalized&&(t=Rt(t,this.array),i=Rt(i,this.array),o=Rt(o,this.array),a=Rt(a,this.array)),this.array[e+0]=t,this.array[e+1]=i,this.array[e+2]=o,this.array[e+3]=a,this}onUpload(e){return this.onUploadCallback=e,this}clone(){return new this.constructor(this.array,this.itemSize).copy(this)}toJSON(){const e={itemSize:this.itemSize,type:this.array.constructor.name,array:Array.from(this.array),normalized:this.normalized};return this.name!==""&&(e.name=this.name),this.usage!==rd&&(e.usage=this.usage),e}}class P_ extends gi{constructor(e,t,i){super(new Uint16Array(e),t,i)}}class b_ extends gi{constructor(e,t,i){super(new Uint32Array(e),t,i)}}class gn extends gi{constructor(e,t,i){super(new Float32Array(e),t,i)}}let US=0;const ti=new kt,lh=new cn,Ks=new W,Yn=new Ca,ca=new Ca,ln=new W;class un extends hs{constructor(){super(),this.isBufferGeometry=!0,Object.defineProperty(this,"id",{value:US++}),this.uuid=Yi(),this.name="",this.type="BufferGeometry",this.index=null,this.attributes={},this.morphAttributes={},this.morphTargetsRelative=!1,this.groups=[],this.boundingBox=null,this.boundingSphere=null,this.drawRange={start:0,count:1/0},this.userData={}}getIndex(){return this.index}setIndex(e){return Array.isArray(e)?this.index=new(E_(e)?b_:P_)(e,1):this.index=e,this}getAttribute(e){return this.attributes[e]}setAttribute(e,t){return this.attributes[e]=t,this}deleteAttribute(e){return delete this.attributes[e],this}hasAttribute(e){return this.attributes[e]!==void 0}addGroup(e,t,i=0){this.groups.push({start:e,count:t,materialIndex:i})}clearGroups(){this.groups=[]}setDrawRange(e,t){this.drawRange.start=e,this.drawRange.count=t}applyMatrix4(e){const t=this.attributes.position;t!==void 0&&(t.applyMatrix4(e),t.needsUpdate=!0);const i=this.attributes.normal;if(i!==void 0){const a=new pt().getNormalMatrix(e);i.applyNormalMatrix(a),i.needsUpdate=!0}const o=this.attributes.tangent;return o!==void 0&&(o.transformDirection(e),o.needsUpdate=!0),this.boundingBox!==null&&this.computeBoundingBox(),this.boundingSphere!==null&&this.computeBoundingSphere(),this}applyQuaternion(e){return ti.makeRotationFromQuaternion(e),this.applyMatrix4(ti),this}rotateX(e){return ti.makeRotationX(e),this.applyMatrix4(ti),this}rotateY(e){return ti.makeRotationY(e),this.applyMatrix4(ti),this}rotateZ(e){return ti.makeRotationZ(e),this.applyMatrix4(ti),this}translate(e,t,i){return ti.makeTranslation(e,t,i),this.applyMatrix4(ti),this}scale(e,t,i){return ti.makeScale(e,t,i),this.applyMatrix4(ti),this}lookAt(e){return lh.lookAt(e),lh.updateMatrix(),this.applyMatrix4(lh.matrix),this}center(){return this.computeBoundingBox(),this.boundingBox.getCenter(Ks).negate(),this.translate(Ks.x,Ks.y,Ks.z),this}setFromPoints(e){const t=[];for(let i=0,o=e.length;i<o;i++){const a=e[i];t.push(a.x,a.y,a.z||0)}return this.setAttribute("position",new gn(t,3)),this}computeBoundingBox(){this.boundingBox===null&&(this.boundingBox=new Ca);const e=this.attributes.position,t=this.morphAttributes.position;if(e&&e.isGLBufferAttribute){console.error("THREE.BufferGeometry.computeBoundingBox(): GLBufferAttribute requires a manual bounding box.",this),this.boundingBox.set(new W(-1/0,-1/0,-1/0),new W(1/0,1/0,1/0));return}if(e!==void 0){if(this.boundingBox.setFromBufferAttribute(e),t)for(let i=0,o=t.length;i<o;i++){const a=t[i];Yn.setFromBufferAttribute(a),this.morphTargetsRelative?(ln.addVectors(this.boundingBox.min,Yn.min),this.boundingBox.expandByPoint(ln),ln.addVectors(this.boundingBox.max,Yn.max),this.boundingBox.expandByPoint(ln)):(this.boundingBox.expandByPoint(Yn.min),this.boundingBox.expandByPoint(Yn.max))}}else this.boundingBox.makeEmpty();(isNaN(this.boundingBox.min.x)||isNaN(this.boundingBox.min.y)||isNaN(this.boundingBox.min.z))&&console.error('THREE.BufferGeometry.computeBoundingBox(): Computed min/max have NaN values. The "position" attribute is likely to have NaN values.',this)}computeBoundingSphere(){this.boundingSphere===null&&(this.boundingSphere=new Iu);const e=this.attributes.position,t=this.morphAttributes.position;if(e&&e.isGLBufferAttribute){console.error("THREE.BufferGeometry.computeBoundingSphere(): GLBufferAttribute requires a manual bounding sphere.",this),this.boundingSphere.set(new W,1/0);return}if(e){const i=this.boundingSphere.center;if(Yn.setFromBufferAttribute(e),t)for(let a=0,u=t.length;a<u;a++){const c=t[a];ca.setFromBufferAttribute(c),this.morphTargetsRelative?(ln.addVectors(Yn.min,ca.min),Yn.expandByPoint(ln),ln.addVectors(Yn.max,ca.max),Yn.expandByPoint(ln)):(Yn.expandByPoint(ca.min),Yn.expandByPoint(ca.max))}Yn.getCenter(i);let o=0;for(let a=0,u=e.count;a<u;a++)ln.fromBufferAttribute(e,a),o=Math.max(o,i.distanceToSquared(ln));if(t)for(let a=0,u=t.length;a<u;a++){const c=t[a],h=this.morphTargetsRelative;for(let d=0,m=c.count;d<m;d++)ln.fromBufferAttribute(c,d),h&&(Ks.fromBufferAttribute(e,d),ln.add(Ks)),o=Math.max(o,i.distanceToSquared(ln))}this.boundingSphere.radius=Math.sqrt(o),isNaN(this.boundingSphere.radius)&&console.error('THREE.BufferGeometry.computeBoundingSphere(): Computed radius is NaN. The "position" attribute is likely to have NaN values.',this)}}computeTangents(){const e=this.index,t=this.attributes;if(e===null||t.position===void 0||t.normal===void 0||t.uv===void 0){console.error("THREE.BufferGeometry: .computeTangents() failed. Missing required attributes (index, position, normal or uv)");return}const i=t.position,o=t.normal,a=t.uv;this.hasAttribute("tangent")===!1&&this.setAttribute("tangent",new gi(new Float32Array(4*i.count),4));const u=this.getAttribute("tangent"),c=[],h=[];for(let z=0;z<i.count;z++)c[z]=new W,h[z]=new W;const d=new W,m=new W,_=new W,g=new Pe,S=new Pe,M=new Pe,E=new W,y=new W;function x(z,C,w){d.fromBufferAttribute(i,z),m.fromBufferAttribute(i,C),_.fromBufferAttribute(i,w),g.fromBufferAttribute(a,z),S.fromBufferAttribute(a,C),M.fromBufferAttribute(a,w),m.sub(d),_.sub(d),S.sub(g),M.sub(g);const U=1/(S.x*M.y-M.x*S.y);isFinite(U)&&(E.copy(m).multiplyScalar(M.y).addScaledVector(_,-S.y).multiplyScalar(U),y.copy(_).multiplyScalar(S.x).addScaledVector(m,-M.x).multiplyScalar(U),c[z].add(E),c[C].add(E),c[w].add(E),h[z].add(y),h[C].add(y),h[w].add(y))}let b=this.groups;b.length===0&&(b=[{start:0,count:e.count}]);for(let z=0,C=b.length;z<C;++z){const w=b[z],U=w.start,X=w.count;for(let k=U,Y=U+X;k<Y;k+=3)x(e.getX(k+0),e.getX(k+1),e.getX(k+2))}const T=new W,P=new W,H=new W,D=new W;function N(z){H.fromBufferAttribute(o,z),D.copy(H);const C=c[z];T.copy(C),T.sub(H.multiplyScalar(H.dot(C))).normalize(),P.crossVectors(D,C);const U=P.dot(h[z])<0?-1:1;u.setXYZW(z,T.x,T.y,T.z,U)}for(let z=0,C=b.length;z<C;++z){const w=b[z],U=w.start,X=w.count;for(let k=U,Y=U+X;k<Y;k+=3)N(e.getX(k+0)),N(e.getX(k+1)),N(e.getX(k+2))}}computeVertexNormals(){const e=this.index,t=this.getAttribute("position");if(t!==void 0){let i=this.getAttribute("normal");if(i===void 0)i=new gi(new Float32Array(t.count*3),3),this.setAttribute("normal",i);else for(let g=0,S=i.count;g<S;g++)i.setXYZ(g,0,0,0);const o=new W,a=new W,u=new W,c=new W,h=new W,d=new W,m=new W,_=new W;if(e)for(let g=0,S=e.count;g<S;g+=3){const M=e.getX(g+0),E=e.getX(g+1),y=e.getX(g+2);o.fromBufferAttribute(t,M),a.fromBufferAttribute(t,E),u.fromBufferAttribute(t,y),m.subVectors(u,a),_.subVectors(o,a),m.cross(_),c.fromBufferAttribute(i,M),h.fromBufferAttribute(i,E),d.fromBufferAttribute(i,y),c.add(m),h.add(m),d.add(m),i.setXYZ(M,c.x,c.y,c.z),i.setXYZ(E,h.x,h.y,h.z),i.setXYZ(y,d.x,d.y,d.z)}else for(let g=0,S=t.count;g<S;g+=3)o.fromBufferAttribute(t,g+0),a.fromBufferAttribute(t,g+1),u.fromBufferAttribute(t,g+2),m.subVectors(u,a),_.subVectors(o,a),m.cross(_),i.setXYZ(g+0,m.x,m.y,m.z),i.setXYZ(g+1,m.x,m.y,m.z),i.setXYZ(g+2,m.x,m.y,m.z);this.normalizeNormals(),i.needsUpdate=!0}}normalizeNormals(){const e=this.attributes.normal;for(let t=0,i=e.count;t<i;t++)ln.fromBufferAttribute(e,t),ln.normalize(),e.setXYZ(t,ln.x,ln.y,ln.z)}toNonIndexed(){function e(c,h){const d=c.array,m=c.itemSize,_=c.normalized,g=new d.constructor(h.length*m);let S=0,M=0;for(let E=0,y=h.length;E<y;E++){c.isInterleavedBufferAttribute?S=h[E]*c.data.stride+c.offset:S=h[E]*m;for(let x=0;x<m;x++)g[M++]=d[S++]}return new gi(g,m,_)}if(this.index===null)return console.warn("THREE.BufferGeometry.toNonIndexed(): BufferGeometry is already non-indexed."),this;const t=new un,i=this.index.array,o=this.attributes;for(const c in o){const h=o[c],d=e(h,i);t.setAttribute(c,d)}const a=this.morphAttributes;for(const c in a){const h=[],d=a[c];for(let m=0,_=d.length;m<_;m++){const g=d[m],S=e(g,i);h.push(S)}t.morphAttributes[c]=h}t.morphTargetsRelative=this.morphTargetsRelative;const u=this.groups;for(let c=0,h=u.length;c<h;c++){const d=u[c];t.addGroup(d.start,d.count,d.materialIndex)}return t}toJSON(){const e={metadata:{version:4.6,type:"BufferGeometry",generator:"BufferGeometry.toJSON"}};if(e.uuid=this.uuid,e.type=this.type,this.name!==""&&(e.name=this.name),Object.keys(this.userData).length>0&&(e.userData=this.userData),this.parameters!==void 0){const h=this.parameters;for(const d in h)h[d]!==void 0&&(e[d]=h[d]);return e}e.data={attributes:{}};const t=this.index;t!==null&&(e.data.index={type:t.array.constructor.name,array:Array.prototype.slice.call(t.array)});const i=this.attributes;for(const h in i){const d=i[h];e.data.attributes[h]=d.toJSON(e.data)}const o={};let a=!1;for(const h in this.morphAttributes){const d=this.morphAttributes[h],m=[];for(let _=0,g=d.length;_<g;_++){const S=d[_];m.push(S.toJSON(e.data))}m.length>0&&(o[h]=m,a=!0)}a&&(e.data.morphAttributes=o,e.data.morphTargetsRelative=this.morphTargetsRelative);const u=this.groups;u.length>0&&(e.data.groups=JSON.parse(JSON.stringify(u)));const c=this.boundingSphere;return c!==null&&(e.data.boundingSphere={center:c.center.toArray(),radius:c.radius}),e}clone(){return new this.constructor().copy(this)}copy(e){this.index=null,this.attributes={},this.morphAttributes={},this.groups=[],this.boundingBox=null,this.boundingSphere=null;const t={};this.name=e.name;const i=e.index;i!==null&&this.setIndex(i.clone(t));const o=e.attributes;for(const d in o){const m=o[d];this.setAttribute(d,m.clone(t))}const a=e.morphAttributes;for(const d in a){const m=[],_=a[d];for(let g=0,S=_.length;g<S;g++)m.push(_[g].clone(t));this.morphAttributes[d]=m}this.morphTargetsRelative=e.morphTargetsRelative;const u=e.groups;for(let d=0,m=u.length;d<m;d++){const _=u[d];this.addGroup(_.start,_.count,_.materialIndex)}const c=e.boundingBox;c!==null&&(this.boundingBox=c.clone());const h=e.boundingSphere;return h!==null&&(this.boundingSphere=h.clone()),this.drawRange.start=e.drawRange.start,this.drawRange.count=e.drawRange.count,this.userData=e.userData,this}dispose(){this.dispatchEvent({type:"dispose"})}}const r0=new kt,Zr=new Sd,jl=new Iu,s0=new W,$s=new W,Zs=new W,Js=new W,uh=new W,ql=new W,Yl=new Pe,Kl=new Pe,$l=new Pe,o0=new W,a0=new W,l0=new W,Zl=new W,Jl=new W;class zt extends cn{constructor(e=new un,t=new R_){super(),this.isMesh=!0,this.type="Mesh",this.geometry=e,this.material=t,this.updateMorphTargets()}copy(e,t){return super.copy(e,t),e.morphTargetInfluences!==void 0&&(this.morphTargetInfluences=e.morphTargetInfluences.slice()),e.morphTargetDictionary!==void 0&&(this.morphTargetDictionary=Object.assign({},e.morphTargetDictionary)),this.material=Array.isArray(e.material)?e.material.slice():e.material,this.geometry=e.geometry,this}updateMorphTargets(){const t=this.geometry.morphAttributes,i=Object.keys(t);if(i.length>0){const o=t[i[0]];if(o!==void 0){this.morphTargetInfluences=[],this.morphTargetDictionary={};for(let a=0,u=o.length;a<u;a++){const c=o[a].name||String(a);this.morphTargetInfluences.push(0),this.morphTargetDictionary[c]=a}}}}getVertexPosition(e,t){const i=this.geometry,o=i.attributes.position,a=i.morphAttributes.position,u=i.morphTargetsRelative;t.fromBufferAttribute(o,e);const c=this.morphTargetInfluences;if(a&&c){ql.set(0,0,0);for(let h=0,d=a.length;h<d;h++){const m=c[h],_=a[h];m!==0&&(uh.fromBufferAttribute(_,e),u?ql.addScaledVector(uh,m):ql.addScaledVector(uh.sub(t),m))}t.add(ql)}return t}raycast(e,t){const i=this.geometry,o=this.material,a=this.matrixWorld;o!==void 0&&(i.boundingSphere===null&&i.computeBoundingSphere(),jl.copy(i.boundingSphere),jl.applyMatrix4(a),Zr.copy(e.ray).recast(e.near),!(jl.containsPoint(Zr.origin)===!1&&(Zr.intersectSphere(jl,s0)===null||Zr.origin.distanceToSquared(s0)>(e.far-e.near)**2))&&(r0.copy(a).invert(),Zr.copy(e.ray).applyMatrix4(r0),!(i.boundingBox!==null&&Zr.intersectsBox(i.boundingBox)===!1)&&this._computeIntersections(e,t,Zr)))}_computeIntersections(e,t,i){let o;const a=this.geometry,u=this.material,c=a.index,h=a.attributes.position,d=a.attributes.uv,m=a.attributes.uv1,_=a.attributes.normal,g=a.groups,S=a.drawRange;if(c!==null)if(Array.isArray(u))for(let M=0,E=g.length;M<E;M++){const y=g[M],x=u[y.materialIndex],b=Math.max(y.start,S.start),T=Math.min(c.count,Math.min(y.start+y.count,S.start+S.count));for(let P=b,H=T;P<H;P+=3){const D=c.getX(P),N=c.getX(P+1),z=c.getX(P+2);o=Ql(this,x,e,i,d,m,_,D,N,z),o&&(o.faceIndex=Math.floor(P/3),o.face.materialIndex=y.materialIndex,t.push(o))}}else{const M=Math.max(0,S.start),E=Math.min(c.count,S.start+S.count);for(let y=M,x=E;y<x;y+=3){const b=c.getX(y),T=c.getX(y+1),P=c.getX(y+2);o=Ql(this,u,e,i,d,m,_,b,T,P),o&&(o.faceIndex=Math.floor(y/3),t.push(o))}}else if(h!==void 0)if(Array.isArray(u))for(let M=0,E=g.length;M<E;M++){const y=g[M],x=u[y.materialIndex],b=Math.max(y.start,S.start),T=Math.min(h.count,Math.min(y.start+y.count,S.start+S.count));for(let P=b,H=T;P<H;P+=3){const D=P,N=P+1,z=P+2;o=Ql(this,x,e,i,d,m,_,D,N,z),o&&(o.faceIndex=Math.floor(P/3),o.face.materialIndex=y.materialIndex,t.push(o))}}else{const M=Math.max(0,S.start),E=Math.min(h.count,S.start+S.count);for(let y=M,x=E;y<x;y+=3){const b=y,T=y+1,P=y+2;o=Ql(this,u,e,i,d,m,_,b,T,P),o&&(o.faceIndex=Math.floor(y/3),t.push(o))}}}}function FS(s,e,t,i,o,a,u,c){let h;if(e.side===Bn?h=i.intersectTriangle(u,a,o,!0,c):h=i.intersectTriangle(o,a,u,e.side===Dr,c),h===null)return null;Jl.copy(c),Jl.applyMatrix4(s.matrixWorld);const d=t.ray.origin.distanceTo(Jl);return d<t.near||d>t.far?null:{distance:d,point:Jl.clone(),object:s}}function Ql(s,e,t,i,o,a,u,c,h,d){s.getVertexPosition(c,$s),s.getVertexPosition(h,Zs),s.getVertexPosition(d,Js);const m=FS(s,e,t,i,$s,Zs,Js,Zl);if(m){o&&(Yl.fromBufferAttribute(o,c),Kl.fromBufferAttribute(o,h),$l.fromBufferAttribute(o,d),m.uv=ii.getInterpolation(Zl,$s,Zs,Js,Yl,Kl,$l,new Pe)),a&&(Yl.fromBufferAttribute(a,c),Kl.fromBufferAttribute(a,h),$l.fromBufferAttribute(a,d),m.uv1=ii.getInterpolation(Zl,$s,Zs,Js,Yl,Kl,$l,new Pe)),u&&(o0.fromBufferAttribute(u,c),a0.fromBufferAttribute(u,h),l0.fromBufferAttribute(u,d),m.normal=ii.getInterpolation(Zl,$s,Zs,Js,o0,a0,l0,new W),m.normal.dot(i.direction)>0&&m.normal.multiplyScalar(-1));const _={a:c,b:h,c:d,normal:new W,materialIndex:0};ii.getNormal($s,Zs,Js,_.normal),m.face=_}return m}class On extends un{constructor(e=1,t=1,i=1,o=1,a=1,u=1){super(),this.type="BoxGeometry",this.parameters={width:e,height:t,depth:i,widthSegments:o,heightSegments:a,depthSegments:u};const c=this;o=Math.floor(o),a=Math.floor(a),u=Math.floor(u);const h=[],d=[],m=[],_=[];let g=0,S=0;M("z","y","x",-1,-1,i,t,e,u,a,0),M("z","y","x",1,-1,i,t,-e,u,a,1),M("x","z","y",1,1,e,i,t,o,u,2),M("x","z","y",1,-1,e,i,-t,o,u,3),M("x","y","z",1,-1,e,t,i,o,a,4),M("x","y","z",-1,-1,e,t,-i,o,a,5),this.setIndex(h),this.setAttribute("position",new gn(d,3)),this.setAttribute("normal",new gn(m,3)),this.setAttribute("uv",new gn(_,2));function M(E,y,x,b,T,P,H,D,N,z,C){const w=P/N,U=H/z,X=P/2,k=H/2,Y=D/2,ae=N+1,te=z+1;let ce=0,G=0;const he=new W;for(let re=0;re<te;re++){const O=re*U-k;for(let ie=0;ie<ae;ie++){const Ae=ie*w-X;he[E]=Ae*b,he[y]=O*T,he[x]=Y,d.push(he.x,he.y,he.z),he[E]=0,he[y]=0,he[x]=D>0?1:-1,m.push(he.x,he.y,he.z),_.push(ie/N),_.push(1-re/z),ce+=1}}for(let re=0;re<z;re++)for(let O=0;O<N;O++){const ie=g+O+ae*re,Ae=g+O+ae*(re+1),Z=g+(O+1)+ae*(re+1),ne=g+(O+1)+ae*re;h.push(ie,Ae,ne),h.push(Ae,Z,ne),G+=6}c.addGroup(S,G,C),S+=G,g+=ce}}copy(e){return super.copy(e),this.parameters=Object.assign({},e.parameters),this}static fromJSON(e){return new On(e.width,e.height,e.depth,e.widthSegments,e.heightSegments,e.depthSegments)}}function Eo(s){const e={};for(const t in s){e[t]={};for(const i in s[t]){const o=s[t][i];o&&(o.isColor||o.isMatrix3||o.isMatrix4||o.isVector2||o.isVector3||o.isVector4||o.isTexture||o.isQuaternion)?o.isRenderTargetTexture?(console.warn("UniformsUtils: Textures of render targets cannot be cloned via cloneUniforms() or mergeUniforms()."),e[t][i]=null):e[t][i]=o.clone():Array.isArray(o)?e[t][i]=o.slice():e[t][i]=o}}return e}function An(s){const e={};for(let t=0;t<s.length;t++){const i=Eo(s[t]);for(const o in i)e[o]=i[o]}return e}function OS(s){const e=[];for(let t=0;t<s.length;t++)e.push(s[t].clone());return e}function L_(s){const e=s.getRenderTarget();return e===null?s.outputColorSpace:e.isXRRenderTarget===!0?e.texture.colorSpace:Tt.workingColorSpace}const BS={clone:Eo,merge:An};var zS=`void main() {
	gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );
}`,kS=`void main() {
	gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
}`;class Nr extends ds{constructor(e){super(),this.isShaderMaterial=!0,this.type="ShaderMaterial",this.defines={},this.uniforms={},this.uniformsGroups=[],this.vertexShader=zS,this.fragmentShader=kS,this.linewidth=1,this.wireframe=!1,this.wireframeLinewidth=1,this.fog=!1,this.lights=!1,this.clipping=!1,this.forceSinglePass=!0,this.extensions={clipCullDistance:!1,multiDraw:!1},this.defaultAttributeValues={color:[1,1,1],uv:[0,0],uv1:[0,0]},this.index0AttributeName=void 0,this.uniformsNeedUpdate=!1,this.glslVersion=null,e!==void 0&&this.setValues(e)}copy(e){return super.copy(e),this.fragmentShader=e.fragmentShader,this.vertexShader=e.vertexShader,this.uniforms=Eo(e.uniforms),this.uniformsGroups=OS(e.uniformsGroups),this.defines=Object.assign({},e.defines),this.wireframe=e.wireframe,this.wireframeLinewidth=e.wireframeLinewidth,this.fog=e.fog,this.lights=e.lights,this.clipping=e.clipping,this.extensions=Object.assign({},e.extensions),this.glslVersion=e.glslVersion,this}toJSON(e){const t=super.toJSON(e);t.glslVersion=this.glslVersion,t.uniforms={};for(const o in this.uniforms){const u=this.uniforms[o].value;u&&u.isTexture?t.uniforms[o]={type:"t",value:u.toJSON(e).uuid}:u&&u.isColor?t.uniforms[o]={type:"c",value:u.getHex()}:u&&u.isVector2?t.uniforms[o]={type:"v2",value:u.toArray()}:u&&u.isVector3?t.uniforms[o]={type:"v3",value:u.toArray()}:u&&u.isVector4?t.uniforms[o]={type:"v4",value:u.toArray()}:u&&u.isMatrix3?t.uniforms[o]={type:"m3",value:u.toArray()}:u&&u.isMatrix4?t.uniforms[o]={type:"m4",value:u.toArray()}:t.uniforms[o]={value:u}}Object.keys(this.defines).length>0&&(t.defines=this.defines),t.vertexShader=this.vertexShader,t.fragmentShader=this.fragmentShader,t.lights=this.lights,t.clipping=this.clipping;const i={};for(const o in this.extensions)this.extensions[o]===!0&&(i[o]=!0);return Object.keys(i).length>0&&(t.extensions=i),t}}class D_ extends cn{constructor(){super(),this.isCamera=!0,this.type="Camera",this.matrixWorldInverse=new kt,this.projectionMatrix=new kt,this.projectionMatrixInverse=new kt,this.coordinateSystem=ji}copy(e,t){return super.copy(e,t),this.matrixWorldInverse.copy(e.matrixWorldInverse),this.projectionMatrix.copy(e.projectionMatrix),this.projectionMatrixInverse.copy(e.projectionMatrixInverse),this.coordinateSystem=e.coordinateSystem,this}getWorldDirection(e){return super.getWorldDirection(e).negate()}updateMatrixWorld(e){super.updateMatrixWorld(e),this.matrixWorldInverse.copy(this.matrixWorld).invert()}updateWorldMatrix(e,t){super.updateWorldMatrix(e,t),this.matrixWorldInverse.copy(this.matrixWorld).invert()}clone(){return new this.constructor().copy(this)}}const Ar=new W,u0=new Pe,c0=new Pe;class ni extends D_{constructor(e=50,t=1,i=.1,o=2e3){super(),this.isPerspectiveCamera=!0,this.type="PerspectiveCamera",this.fov=e,this.zoom=1,this.near=i,this.far=o,this.focus=10,this.aspect=t,this.view=null,this.filmGauge=35,this.filmOffset=0,this.updateProjectionMatrix()}copy(e,t){return super.copy(e,t),this.fov=e.fov,this.zoom=e.zoom,this.near=e.near,this.far=e.far,this.focus=e.focus,this.aspect=e.aspect,this.view=e.view===null?null:Object.assign({},e.view),this.filmGauge=e.filmGauge,this.filmOffset=e.filmOffset,this}setFocalLength(e){const t=.5*this.getFilmHeight()/e;this.fov=sd*2*Math.atan(t),this.updateProjectionMatrix()}getFocalLength(){const e=Math.tan(_a*.5*this.fov);return .5*this.getFilmHeight()/e}getEffectiveFOV(){return sd*2*Math.atan(Math.tan(_a*.5*this.fov)/this.zoom)}getFilmWidth(){return this.filmGauge*Math.min(this.aspect,1)}getFilmHeight(){return this.filmGauge/Math.max(this.aspect,1)}getViewBounds(e,t,i){Ar.set(-1,-1,.5).applyMatrix4(this.projectionMatrixInverse),t.set(Ar.x,Ar.y).multiplyScalar(-e/Ar.z),Ar.set(1,1,.5).applyMatrix4(this.projectionMatrixInverse),i.set(Ar.x,Ar.y).multiplyScalar(-e/Ar.z)}getViewSize(e,t){return this.getViewBounds(e,u0,c0),t.subVectors(c0,u0)}setViewOffset(e,t,i,o,a,u){this.aspect=e/t,this.view===null&&(this.view={enabled:!0,fullWidth:1,fullHeight:1,offsetX:0,offsetY:0,width:1,height:1}),this.view.enabled=!0,this.view.fullWidth=e,this.view.fullHeight=t,this.view.offsetX=i,this.view.offsetY=o,this.view.width=a,this.view.height=u,this.updateProjectionMatrix()}clearViewOffset(){this.view!==null&&(this.view.enabled=!1),this.updateProjectionMatrix()}updateProjectionMatrix(){const e=this.near;let t=e*Math.tan(_a*.5*this.fov)/this.zoom,i=2*t,o=this.aspect*i,a=-.5*o;const u=this.view;if(this.view!==null&&this.view.enabled){const h=u.fullWidth,d=u.fullHeight;a+=u.offsetX*o/h,t-=u.offsetY*i/d,o*=u.width/h,i*=u.height/d}const c=this.filmOffset;c!==0&&(a+=e*c/this.getFilmWidth()),this.projectionMatrix.makePerspective(a,a+o,t,t-i,e,this.far,this.coordinateSystem),this.projectionMatrixInverse.copy(this.projectionMatrix).invert()}toJSON(e){const t=super.toJSON(e);return t.object.fov=this.fov,t.object.zoom=this.zoom,t.object.near=this.near,t.object.far=this.far,t.object.focus=this.focus,t.object.aspect=this.aspect,this.view!==null&&(t.object.view=Object.assign({},this.view)),t.object.filmGauge=this.filmGauge,t.object.filmOffset=this.filmOffset,t}}const Qs=-90,eo=1;class HS extends cn{constructor(e,t,i){super(),this.type="CubeCamera",this.renderTarget=i,this.coordinateSystem=null,this.activeMipmapLevel=0;const o=new ni(Qs,eo,e,t);o.layers=this.layers,this.add(o);const a=new ni(Qs,eo,e,t);a.layers=this.layers,this.add(a);const u=new ni(Qs,eo,e,t);u.layers=this.layers,this.add(u);const c=new ni(Qs,eo,e,t);c.layers=this.layers,this.add(c);const h=new ni(Qs,eo,e,t);h.layers=this.layers,this.add(h);const d=new ni(Qs,eo,e,t);d.layers=this.layers,this.add(d)}updateCoordinateSystem(){const e=this.coordinateSystem,t=this.children.concat(),[i,o,a,u,c,h]=t;for(const d of t)this.remove(d);if(e===ji)i.up.set(0,1,0),i.lookAt(1,0,0),o.up.set(0,1,0),o.lookAt(-1,0,0),a.up.set(0,0,-1),a.lookAt(0,1,0),u.up.set(0,0,1),u.lookAt(0,-1,0),c.up.set(0,1,0),c.lookAt(0,0,1),h.up.set(0,1,0),h.lookAt(0,0,-1);else if(e===Cu)i.up.set(0,-1,0),i.lookAt(-1,0,0),o.up.set(0,-1,0),o.lookAt(1,0,0),a.up.set(0,0,1),a.lookAt(0,1,0),u.up.set(0,0,-1),u.lookAt(0,-1,0),c.up.set(0,-1,0),c.lookAt(0,0,1),h.up.set(0,-1,0),h.lookAt(0,0,-1);else throw new Error("THREE.CubeCamera.updateCoordinateSystem(): Invalid coordinate system: "+e);for(const d of t)this.add(d),d.updateMatrixWorld()}update(e,t){this.parent===null&&this.updateMatrixWorld();const{renderTarget:i,activeMipmapLevel:o}=this;this.coordinateSystem!==e.coordinateSystem&&(this.coordinateSystem=e.coordinateSystem,this.updateCoordinateSystem());const[a,u,c,h,d,m]=this.children,_=e.getRenderTarget(),g=e.getActiveCubeFace(),S=e.getActiveMipmapLevel(),M=e.xr.enabled;e.xr.enabled=!1;const E=i.texture.generateMipmaps;i.texture.generateMipmaps=!1,e.setRenderTarget(i,0,o),e.render(t,a),e.setRenderTarget(i,1,o),e.render(t,u),e.setRenderTarget(i,2,o),e.render(t,c),e.setRenderTarget(i,3,o),e.render(t,h),e.setRenderTarget(i,4,o),e.render(t,d),i.texture.generateMipmaps=E,e.setRenderTarget(i,5,o),e.render(t,m),e.setRenderTarget(_,g,S),e.xr.enabled=M,i.texture.needsPMREMUpdate=!0}}class N_ extends Rn{constructor(e,t,i,o,a,u,c,h,d,m){e=e!==void 0?e:[],t=t!==void 0?t:yo,super(e,t,i,o,a,u,c,h,d,m),this.isCubeTexture=!0,this.flipY=!1}get images(){return this.image}set images(e){this.image=e}}class VS extends as{constructor(e=1,t={}){super(e,e,t),this.isWebGLCubeRenderTarget=!0;const i={width:e,height:e,depth:1},o=[i,i,i,i,i,i];this.texture=new N_(o,t.mapping,t.wrapS,t.wrapT,t.magFilter,t.minFilter,t.format,t.type,t.anisotropy,t.colorSpace),this.texture.isRenderTargetTexture=!0,this.texture.generateMipmaps=t.generateMipmaps!==void 0?t.generateMipmaps:!1,this.texture.minFilter=t.minFilter!==void 0?t.minFilter:mn}fromEquirectangularTexture(e,t){this.texture.type=t.type,this.texture.colorSpace=t.colorSpace,this.texture.generateMipmaps=t.generateMipmaps,this.texture.minFilter=t.minFilter,this.texture.magFilter=t.magFilter;const i={uniforms:{tEquirect:{value:null}},vertexShader:`

				varying vec3 vWorldDirection;

				vec3 transformDirection( in vec3 dir, in mat4 matrix ) {

					return normalize( ( matrix * vec4( dir, 0.0 ) ).xyz );

				}

				void main() {

					vWorldDirection = transformDirection( position, modelMatrix );

					#include <begin_vertex>
					#include <project_vertex>

				}
			`,fragmentShader:`

				uniform sampler2D tEquirect;

				varying vec3 vWorldDirection;

				#include <common>

				void main() {

					vec3 direction = normalize( vWorldDirection );

					vec2 sampleUV = equirectUv( direction );

					gl_FragColor = texture2D( tEquirect, sampleUV );

				}
			`},o=new On(5,5,5),a=new Nr({name:"CubemapFromEquirect",uniforms:Eo(i.uniforms),vertexShader:i.vertexShader,fragmentShader:i.fragmentShader,side:Bn,blending:br});a.uniforms.tEquirect.value=t;const u=new zt(o,a),c=t.minFilter;return t.minFilter===ss&&(t.minFilter=mn),new HS(1,10,this).update(e,u),t.minFilter=c,u.geometry.dispose(),u.material.dispose(),this}clear(e,t,i,o){const a=e.getRenderTarget();for(let u=0;u<6;u++)e.setRenderTarget(this,u),e.clear(t,i,o);e.setRenderTarget(a)}}const ch=new W,GS=new W,WS=new pt;class Cr{constructor(e=new W(1,0,0),t=0){this.isPlane=!0,this.normal=e,this.constant=t}set(e,t){return this.normal.copy(e),this.constant=t,this}setComponents(e,t,i,o){return this.normal.set(e,t,i),this.constant=o,this}setFromNormalAndCoplanarPoint(e,t){return this.normal.copy(e),this.constant=-t.dot(this.normal),this}setFromCoplanarPoints(e,t,i){const o=ch.subVectors(i,t).cross(GS.subVectors(e,t)).normalize();return this.setFromNormalAndCoplanarPoint(o,e),this}copy(e){return this.normal.copy(e.normal),this.constant=e.constant,this}normalize(){const e=1/this.normal.length();return this.normal.multiplyScalar(e),this.constant*=e,this}negate(){return this.constant*=-1,this.normal.negate(),this}distanceToPoint(e){return this.normal.dot(e)+this.constant}distanceToSphere(e){return this.distanceToPoint(e.center)-e.radius}projectPoint(e,t){return t.copy(e).addScaledVector(this.normal,-this.distanceToPoint(e))}intersectLine(e,t){const i=e.delta(ch),o=this.normal.dot(i);if(o===0)return this.distanceToPoint(e.start)===0?t.copy(e.start):null;const a=-(e.start.dot(this.normal)+this.constant)/o;return a<0||a>1?null:t.copy(e.start).addScaledVector(i,a)}intersectsLine(e){const t=this.distanceToPoint(e.start),i=this.distanceToPoint(e.end);return t<0&&i>0||i<0&&t>0}intersectsBox(e){return e.intersectsPlane(this)}intersectsSphere(e){return e.intersectsPlane(this)}coplanarPoint(e){return e.copy(this.normal).multiplyScalar(-this.constant)}applyMatrix4(e,t){const i=t||WS.getNormalMatrix(e),o=this.coplanarPoint(ch).applyMatrix4(e),a=this.normal.applyMatrix3(i).normalize();return this.constant=-o.dot(a),this}translate(e){return this.constant-=e.dot(this.normal),this}equals(e){return e.normal.equals(this.normal)&&e.constant===this.constant}clone(){return new this.constructor().copy(this)}}const Jr=new Iu,eu=new W;class Md{constructor(e=new Cr,t=new Cr,i=new Cr,o=new Cr,a=new Cr,u=new Cr){this.planes=[e,t,i,o,a,u]}set(e,t,i,o,a,u){const c=this.planes;return c[0].copy(e),c[1].copy(t),c[2].copy(i),c[3].copy(o),c[4].copy(a),c[5].copy(u),this}copy(e){const t=this.planes;for(let i=0;i<6;i++)t[i].copy(e.planes[i]);return this}setFromProjectionMatrix(e,t=ji){const i=this.planes,o=e.elements,a=o[0],u=o[1],c=o[2],h=o[3],d=o[4],m=o[5],_=o[6],g=o[7],S=o[8],M=o[9],E=o[10],y=o[11],x=o[12],b=o[13],T=o[14],P=o[15];if(i[0].setComponents(h-a,g-d,y-S,P-x).normalize(),i[1].setComponents(h+a,g+d,y+S,P+x).normalize(),i[2].setComponents(h+u,g+m,y+M,P+b).normalize(),i[3].setComponents(h-u,g-m,y-M,P-b).normalize(),i[4].setComponents(h-c,g-_,y-E,P-T).normalize(),t===ji)i[5].setComponents(h+c,g+_,y+E,P+T).normalize();else if(t===Cu)i[5].setComponents(c,_,E,T).normalize();else throw new Error("THREE.Frustum.setFromProjectionMatrix(): Invalid coordinate system: "+t);return this}intersectsObject(e){if(e.boundingSphere!==void 0)e.boundingSphere===null&&e.computeBoundingSphere(),Jr.copy(e.boundingSphere).applyMatrix4(e.matrixWorld);else{const t=e.geometry;t.boundingSphere===null&&t.computeBoundingSphere(),Jr.copy(t.boundingSphere).applyMatrix4(e.matrixWorld)}return this.intersectsSphere(Jr)}intersectsSprite(e){return Jr.center.set(0,0,0),Jr.radius=.7071067811865476,Jr.applyMatrix4(e.matrixWorld),this.intersectsSphere(Jr)}intersectsSphere(e){const t=this.planes,i=e.center,o=-e.radius;for(let a=0;a<6;a++)if(t[a].distanceToPoint(i)<o)return!1;return!0}intersectsBox(e){const t=this.planes;for(let i=0;i<6;i++){const o=t[i];if(eu.x=o.normal.x>0?e.max.x:e.min.x,eu.y=o.normal.y>0?e.max.y:e.min.y,eu.z=o.normal.z>0?e.max.z:e.min.z,o.distanceToPoint(eu)<0)return!1}return!0}containsPoint(e){const t=this.planes;for(let i=0;i<6;i++)if(t[i].distanceToPoint(e)<0)return!1;return!0}clone(){return new this.constructor().copy(this)}}function I_(){let s=null,e=!1,t=null,i=null;function o(a,u){t(a,u),i=s.requestAnimationFrame(o)}return{start:function(){e!==!0&&t!==null&&(i=s.requestAnimationFrame(o),e=!0)},stop:function(){s.cancelAnimationFrame(i),e=!1},setAnimationLoop:function(a){t=a},setContext:function(a){s=a}}}function XS(s){const e=new WeakMap;function t(c,h){const d=c.array,m=c.usage,_=d.byteLength,g=s.createBuffer();s.bindBuffer(h,g),s.bufferData(h,d,m),c.onUploadCallback();let S;if(d instanceof Float32Array)S=s.FLOAT;else if(d instanceof Uint16Array)c.isFloat16BufferAttribute?S=s.HALF_FLOAT:S=s.UNSIGNED_SHORT;else if(d instanceof Int16Array)S=s.SHORT;else if(d instanceof Uint32Array)S=s.UNSIGNED_INT;else if(d instanceof Int32Array)S=s.INT;else if(d instanceof Int8Array)S=s.BYTE;else if(d instanceof Uint8Array)S=s.UNSIGNED_BYTE;else if(d instanceof Uint8ClampedArray)S=s.UNSIGNED_BYTE;else throw new Error("THREE.WebGLAttributes: Unsupported buffer data format: "+d);return{buffer:g,type:S,bytesPerElement:d.BYTES_PER_ELEMENT,version:c.version,size:_}}function i(c,h,d){const m=h.array,_=h._updateRange,g=h.updateRanges;if(s.bindBuffer(d,c),_.count===-1&&g.length===0&&s.bufferSubData(d,0,m),g.length!==0){for(let S=0,M=g.length;S<M;S++){const E=g[S];s.bufferSubData(d,E.start*m.BYTES_PER_ELEMENT,m,E.start,E.count)}h.clearUpdateRanges()}_.count!==-1&&(s.bufferSubData(d,_.offset*m.BYTES_PER_ELEMENT,m,_.offset,_.count),_.count=-1),h.onUploadCallback()}function o(c){return c.isInterleavedBufferAttribute&&(c=c.data),e.get(c)}function a(c){c.isInterleavedBufferAttribute&&(c=c.data);const h=e.get(c);h&&(s.deleteBuffer(h.buffer),e.delete(c))}function u(c,h){if(c.isInterleavedBufferAttribute&&(c=c.data),c.isGLBufferAttribute){const m=e.get(c);(!m||m.version<c.version)&&e.set(c,{buffer:c.buffer,type:c.type,bytesPerElement:c.elementSize,version:c.version});return}const d=e.get(c);if(d===void 0)e.set(c,t(c,h));else if(d.version<c.version){if(d.size!==c.array.byteLength)throw new Error("THREE.WebGLAttributes: The size of the buffer attribute's array buffer does not match the original size. Resizing buffer attributes is not supported.");i(d.buffer,c,h),d.version=c.version}}return{get:o,remove:a,update:u}}class Ra extends un{constructor(e=1,t=1,i=1,o=1){super(),this.type="PlaneGeometry",this.parameters={width:e,height:t,widthSegments:i,heightSegments:o};const a=e/2,u=t/2,c=Math.floor(i),h=Math.floor(o),d=c+1,m=h+1,_=e/c,g=t/h,S=[],M=[],E=[],y=[];for(let x=0;x<m;x++){const b=x*g-u;for(let T=0;T<d;T++){const P=T*_-a;M.push(P,-b,0),E.push(0,0,1),y.push(T/c),y.push(1-x/h)}}for(let x=0;x<h;x++)for(let b=0;b<c;b++){const T=b+d*x,P=b+d*(x+1),H=b+1+d*(x+1),D=b+1+d*x;S.push(T,P,D),S.push(P,H,D)}this.setIndex(S),this.setAttribute("position",new gn(M,3)),this.setAttribute("normal",new gn(E,3)),this.setAttribute("uv",new gn(y,2))}copy(e){return super.copy(e),this.parameters=Object.assign({},e.parameters),this}static fromJSON(e){return new Ra(e.width,e.height,e.widthSegments,e.heightSegments)}}var jS=`#ifdef USE_ALPHAHASH
	if ( diffuseColor.a < getAlphaHashThreshold( vPosition ) ) discard;
#endif`,qS=`#ifdef USE_ALPHAHASH
	const float ALPHA_HASH_SCALE = 0.05;
	float hash2D( vec2 value ) {
		return fract( 1.0e4 * sin( 17.0 * value.x + 0.1 * value.y ) * ( 0.1 + abs( sin( 13.0 * value.y + value.x ) ) ) );
	}
	float hash3D( vec3 value ) {
		return hash2D( vec2( hash2D( value.xy ), value.z ) );
	}
	float getAlphaHashThreshold( vec3 position ) {
		float maxDeriv = max(
			length( dFdx( position.xyz ) ),
			length( dFdy( position.xyz ) )
		);
		float pixScale = 1.0 / ( ALPHA_HASH_SCALE * maxDeriv );
		vec2 pixScales = vec2(
			exp2( floor( log2( pixScale ) ) ),
			exp2( ceil( log2( pixScale ) ) )
		);
		vec2 alpha = vec2(
			hash3D( floor( pixScales.x * position.xyz ) ),
			hash3D( floor( pixScales.y * position.xyz ) )
		);
		float lerpFactor = fract( log2( pixScale ) );
		float x = ( 1.0 - lerpFactor ) * alpha.x + lerpFactor * alpha.y;
		float a = min( lerpFactor, 1.0 - lerpFactor );
		vec3 cases = vec3(
			x * x / ( 2.0 * a * ( 1.0 - a ) ),
			( x - 0.5 * a ) / ( 1.0 - a ),
			1.0 - ( ( 1.0 - x ) * ( 1.0 - x ) / ( 2.0 * a * ( 1.0 - a ) ) )
		);
		float threshold = ( x < ( 1.0 - a ) )
			? ( ( x < a ) ? cases.x : cases.y )
			: cases.z;
		return clamp( threshold , 1.0e-6, 1.0 );
	}
#endif`,YS=`#ifdef USE_ALPHAMAP
	diffuseColor.a *= texture2D( alphaMap, vAlphaMapUv ).g;
#endif`,KS=`#ifdef USE_ALPHAMAP
	uniform sampler2D alphaMap;
#endif`,$S=`#ifdef USE_ALPHATEST
	#ifdef ALPHA_TO_COVERAGE
	diffuseColor.a = smoothstep( alphaTest, alphaTest + fwidth( diffuseColor.a ), diffuseColor.a );
	if ( diffuseColor.a == 0.0 ) discard;
	#else
	if ( diffuseColor.a < alphaTest ) discard;
	#endif
#endif`,ZS=`#ifdef USE_ALPHATEST
	uniform float alphaTest;
#endif`,JS=`#ifdef USE_AOMAP
	float ambientOcclusion = ( texture2D( aoMap, vAoMapUv ).r - 1.0 ) * aoMapIntensity + 1.0;
	reflectedLight.indirectDiffuse *= ambientOcclusion;
	#if defined( USE_CLEARCOAT ) 
		clearcoatSpecularIndirect *= ambientOcclusion;
	#endif
	#if defined( USE_SHEEN ) 
		sheenSpecularIndirect *= ambientOcclusion;
	#endif
	#if defined( USE_ENVMAP ) && defined( STANDARD )
		float dotNV = saturate( dot( geometryNormal, geometryViewDir ) );
		reflectedLight.indirectSpecular *= computeSpecularOcclusion( dotNV, ambientOcclusion, material.roughness );
	#endif
#endif`,QS=`#ifdef USE_AOMAP
	uniform sampler2D aoMap;
	uniform float aoMapIntensity;
#endif`,eM=`#ifdef USE_BATCHING
	#if ! defined( GL_ANGLE_multi_draw )
	#define gl_DrawID _gl_DrawID
	uniform int _gl_DrawID;
	#endif
	uniform highp sampler2D batchingTexture;
	uniform highp usampler2D batchingIdTexture;
	mat4 getBatchingMatrix( const in float i ) {
		int size = textureSize( batchingTexture, 0 ).x;
		int j = int( i ) * 4;
		int x = j % size;
		int y = j / size;
		vec4 v1 = texelFetch( batchingTexture, ivec2( x, y ), 0 );
		vec4 v2 = texelFetch( batchingTexture, ivec2( x + 1, y ), 0 );
		vec4 v3 = texelFetch( batchingTexture, ivec2( x + 2, y ), 0 );
		vec4 v4 = texelFetch( batchingTexture, ivec2( x + 3, y ), 0 );
		return mat4( v1, v2, v3, v4 );
	}
	float getIndirectIndex( const in int i ) {
		int size = textureSize( batchingIdTexture, 0 ).x;
		int x = i % size;
		int y = i / size;
		return float( texelFetch( batchingIdTexture, ivec2( x, y ), 0 ).r );
	}
#endif
#ifdef USE_BATCHING_COLOR
	uniform sampler2D batchingColorTexture;
	vec3 getBatchingColor( const in float i ) {
		int size = textureSize( batchingColorTexture, 0 ).x;
		int j = int( i );
		int x = j % size;
		int y = j / size;
		return texelFetch( batchingColorTexture, ivec2( x, y ), 0 ).rgb;
	}
#endif`,tM=`#ifdef USE_BATCHING
	mat4 batchingMatrix = getBatchingMatrix( getIndirectIndex( gl_DrawID ) );
#endif`,nM=`vec3 transformed = vec3( position );
#ifdef USE_ALPHAHASH
	vPosition = vec3( position );
#endif`,iM=`vec3 objectNormal = vec3( normal );
#ifdef USE_TANGENT
	vec3 objectTangent = vec3( tangent.xyz );
#endif`,rM=`float G_BlinnPhong_Implicit( ) {
	return 0.25;
}
float D_BlinnPhong( const in float shininess, const in float dotNH ) {
	return RECIPROCAL_PI * ( shininess * 0.5 + 1.0 ) * pow( dotNH, shininess );
}
vec3 BRDF_BlinnPhong( const in vec3 lightDir, const in vec3 viewDir, const in vec3 normal, const in vec3 specularColor, const in float shininess ) {
	vec3 halfDir = normalize( lightDir + viewDir );
	float dotNH = saturate( dot( normal, halfDir ) );
	float dotVH = saturate( dot( viewDir, halfDir ) );
	vec3 F = F_Schlick( specularColor, 1.0, dotVH );
	float G = G_BlinnPhong_Implicit( );
	float D = D_BlinnPhong( shininess, dotNH );
	return F * ( G * D );
} // validated`,sM=`#ifdef USE_IRIDESCENCE
	const mat3 XYZ_TO_REC709 = mat3(
		 3.2404542, -0.9692660,  0.0556434,
		-1.5371385,  1.8760108, -0.2040259,
		-0.4985314,  0.0415560,  1.0572252
	);
	vec3 Fresnel0ToIor( vec3 fresnel0 ) {
		vec3 sqrtF0 = sqrt( fresnel0 );
		return ( vec3( 1.0 ) + sqrtF0 ) / ( vec3( 1.0 ) - sqrtF0 );
	}
	vec3 IorToFresnel0( vec3 transmittedIor, float incidentIor ) {
		return pow2( ( transmittedIor - vec3( incidentIor ) ) / ( transmittedIor + vec3( incidentIor ) ) );
	}
	float IorToFresnel0( float transmittedIor, float incidentIor ) {
		return pow2( ( transmittedIor - incidentIor ) / ( transmittedIor + incidentIor ));
	}
	vec3 evalSensitivity( float OPD, vec3 shift ) {
		float phase = 2.0 * PI * OPD * 1.0e-9;
		vec3 val = vec3( 5.4856e-13, 4.4201e-13, 5.2481e-13 );
		vec3 pos = vec3( 1.6810e+06, 1.7953e+06, 2.2084e+06 );
		vec3 var = vec3( 4.3278e+09, 9.3046e+09, 6.6121e+09 );
		vec3 xyz = val * sqrt( 2.0 * PI * var ) * cos( pos * phase + shift ) * exp( - pow2( phase ) * var );
		xyz.x += 9.7470e-14 * sqrt( 2.0 * PI * 4.5282e+09 ) * cos( 2.2399e+06 * phase + shift[ 0 ] ) * exp( - 4.5282e+09 * pow2( phase ) );
		xyz /= 1.0685e-7;
		vec3 rgb = XYZ_TO_REC709 * xyz;
		return rgb;
	}
	vec3 evalIridescence( float outsideIOR, float eta2, float cosTheta1, float thinFilmThickness, vec3 baseF0 ) {
		vec3 I;
		float iridescenceIOR = mix( outsideIOR, eta2, smoothstep( 0.0, 0.03, thinFilmThickness ) );
		float sinTheta2Sq = pow2( outsideIOR / iridescenceIOR ) * ( 1.0 - pow2( cosTheta1 ) );
		float cosTheta2Sq = 1.0 - sinTheta2Sq;
		if ( cosTheta2Sq < 0.0 ) {
			return vec3( 1.0 );
		}
		float cosTheta2 = sqrt( cosTheta2Sq );
		float R0 = IorToFresnel0( iridescenceIOR, outsideIOR );
		float R12 = F_Schlick( R0, 1.0, cosTheta1 );
		float T121 = 1.0 - R12;
		float phi12 = 0.0;
		if ( iridescenceIOR < outsideIOR ) phi12 = PI;
		float phi21 = PI - phi12;
		vec3 baseIOR = Fresnel0ToIor( clamp( baseF0, 0.0, 0.9999 ) );		vec3 R1 = IorToFresnel0( baseIOR, iridescenceIOR );
		vec3 R23 = F_Schlick( R1, 1.0, cosTheta2 );
		vec3 phi23 = vec3( 0.0 );
		if ( baseIOR[ 0 ] < iridescenceIOR ) phi23[ 0 ] = PI;
		if ( baseIOR[ 1 ] < iridescenceIOR ) phi23[ 1 ] = PI;
		if ( baseIOR[ 2 ] < iridescenceIOR ) phi23[ 2 ] = PI;
		float OPD = 2.0 * iridescenceIOR * thinFilmThickness * cosTheta2;
		vec3 phi = vec3( phi21 ) + phi23;
		vec3 R123 = clamp( R12 * R23, 1e-5, 0.9999 );
		vec3 r123 = sqrt( R123 );
		vec3 Rs = pow2( T121 ) * R23 / ( vec3( 1.0 ) - R123 );
		vec3 C0 = R12 + Rs;
		I = C0;
		vec3 Cm = Rs - T121;
		for ( int m = 1; m <= 2; ++ m ) {
			Cm *= r123;
			vec3 Sm = 2.0 * evalSensitivity( float( m ) * OPD, float( m ) * phi );
			I += Cm * Sm;
		}
		return max( I, vec3( 0.0 ) );
	}
#endif`,oM=`#ifdef USE_BUMPMAP
	uniform sampler2D bumpMap;
	uniform float bumpScale;
	vec2 dHdxy_fwd() {
		vec2 dSTdx = dFdx( vBumpMapUv );
		vec2 dSTdy = dFdy( vBumpMapUv );
		float Hll = bumpScale * texture2D( bumpMap, vBumpMapUv ).x;
		float dBx = bumpScale * texture2D( bumpMap, vBumpMapUv + dSTdx ).x - Hll;
		float dBy = bumpScale * texture2D( bumpMap, vBumpMapUv + dSTdy ).x - Hll;
		return vec2( dBx, dBy );
	}
	vec3 perturbNormalArb( vec3 surf_pos, vec3 surf_norm, vec2 dHdxy, float faceDirection ) {
		vec3 vSigmaX = normalize( dFdx( surf_pos.xyz ) );
		vec3 vSigmaY = normalize( dFdy( surf_pos.xyz ) );
		vec3 vN = surf_norm;
		vec3 R1 = cross( vSigmaY, vN );
		vec3 R2 = cross( vN, vSigmaX );
		float fDet = dot( vSigmaX, R1 ) * faceDirection;
		vec3 vGrad = sign( fDet ) * ( dHdxy.x * R1 + dHdxy.y * R2 );
		return normalize( abs( fDet ) * surf_norm - vGrad );
	}
#endif`,aM=`#if NUM_CLIPPING_PLANES > 0
	vec4 plane;
	#ifdef ALPHA_TO_COVERAGE
		float distanceToPlane, distanceGradient;
		float clipOpacity = 1.0;
		#pragma unroll_loop_start
		for ( int i = 0; i < UNION_CLIPPING_PLANES; i ++ ) {
			plane = clippingPlanes[ i ];
			distanceToPlane = - dot( vClipPosition, plane.xyz ) + plane.w;
			distanceGradient = fwidth( distanceToPlane ) / 2.0;
			clipOpacity *= smoothstep( - distanceGradient, distanceGradient, distanceToPlane );
			if ( clipOpacity == 0.0 ) discard;
		}
		#pragma unroll_loop_end
		#if UNION_CLIPPING_PLANES < NUM_CLIPPING_PLANES
			float unionClipOpacity = 1.0;
			#pragma unroll_loop_start
			for ( int i = UNION_CLIPPING_PLANES; i < NUM_CLIPPING_PLANES; i ++ ) {
				plane = clippingPlanes[ i ];
				distanceToPlane = - dot( vClipPosition, plane.xyz ) + plane.w;
				distanceGradient = fwidth( distanceToPlane ) / 2.0;
				unionClipOpacity *= 1.0 - smoothstep( - distanceGradient, distanceGradient, distanceToPlane );
			}
			#pragma unroll_loop_end
			clipOpacity *= 1.0 - unionClipOpacity;
		#endif
		diffuseColor.a *= clipOpacity;
		if ( diffuseColor.a == 0.0 ) discard;
	#else
		#pragma unroll_loop_start
		for ( int i = 0; i < UNION_CLIPPING_PLANES; i ++ ) {
			plane = clippingPlanes[ i ];
			if ( dot( vClipPosition, plane.xyz ) > plane.w ) discard;
		}
		#pragma unroll_loop_end
		#if UNION_CLIPPING_PLANES < NUM_CLIPPING_PLANES
			bool clipped = true;
			#pragma unroll_loop_start
			for ( int i = UNION_CLIPPING_PLANES; i < NUM_CLIPPING_PLANES; i ++ ) {
				plane = clippingPlanes[ i ];
				clipped = ( dot( vClipPosition, plane.xyz ) > plane.w ) && clipped;
			}
			#pragma unroll_loop_end
			if ( clipped ) discard;
		#endif
	#endif
#endif`,lM=`#if NUM_CLIPPING_PLANES > 0
	varying vec3 vClipPosition;
	uniform vec4 clippingPlanes[ NUM_CLIPPING_PLANES ];
#endif`,uM=`#if NUM_CLIPPING_PLANES > 0
	varying vec3 vClipPosition;
#endif`,cM=`#if NUM_CLIPPING_PLANES > 0
	vClipPosition = - mvPosition.xyz;
#endif`,fM=`#if defined( USE_COLOR_ALPHA )
	diffuseColor *= vColor;
#elif defined( USE_COLOR )
	diffuseColor.rgb *= vColor;
#endif`,hM=`#if defined( USE_COLOR_ALPHA )
	varying vec4 vColor;
#elif defined( USE_COLOR )
	varying vec3 vColor;
#endif`,dM=`#if defined( USE_COLOR_ALPHA )
	varying vec4 vColor;
#elif defined( USE_COLOR ) || defined( USE_INSTANCING_COLOR ) || defined( USE_BATCHING_COLOR )
	varying vec3 vColor;
#endif`,pM=`#if defined( USE_COLOR_ALPHA )
	vColor = vec4( 1.0 );
#elif defined( USE_COLOR ) || defined( USE_INSTANCING_COLOR ) || defined( USE_BATCHING_COLOR )
	vColor = vec3( 1.0 );
#endif
#ifdef USE_COLOR
	vColor *= color;
#endif
#ifdef USE_INSTANCING_COLOR
	vColor.xyz *= instanceColor.xyz;
#endif
#ifdef USE_BATCHING_COLOR
	vec3 batchingColor = getBatchingColor( getIndirectIndex( gl_DrawID ) );
	vColor.xyz *= batchingColor.xyz;
#endif`,mM=`#define PI 3.141592653589793
#define PI2 6.283185307179586
#define PI_HALF 1.5707963267948966
#define RECIPROCAL_PI 0.3183098861837907
#define RECIPROCAL_PI2 0.15915494309189535
#define EPSILON 1e-6
#ifndef saturate
#define saturate( a ) clamp( a, 0.0, 1.0 )
#endif
#define whiteComplement( a ) ( 1.0 - saturate( a ) )
float pow2( const in float x ) { return x*x; }
vec3 pow2( const in vec3 x ) { return x*x; }
float pow3( const in float x ) { return x*x*x; }
float pow4( const in float x ) { float x2 = x*x; return x2*x2; }
float max3( const in vec3 v ) { return max( max( v.x, v.y ), v.z ); }
float average( const in vec3 v ) { return dot( v, vec3( 0.3333333 ) ); }
highp float rand( const in vec2 uv ) {
	const highp float a = 12.9898, b = 78.233, c = 43758.5453;
	highp float dt = dot( uv.xy, vec2( a,b ) ), sn = mod( dt, PI );
	return fract( sin( sn ) * c );
}
#ifdef HIGH_PRECISION
	float precisionSafeLength( vec3 v ) { return length( v ); }
#else
	float precisionSafeLength( vec3 v ) {
		float maxComponent = max3( abs( v ) );
		return length( v / maxComponent ) * maxComponent;
	}
#endif
struct IncidentLight {
	vec3 color;
	vec3 direction;
	bool visible;
};
struct ReflectedLight {
	vec3 directDiffuse;
	vec3 directSpecular;
	vec3 indirectDiffuse;
	vec3 indirectSpecular;
};
#ifdef USE_ALPHAHASH
	varying vec3 vPosition;
#endif
vec3 transformDirection( in vec3 dir, in mat4 matrix ) {
	return normalize( ( matrix * vec4( dir, 0.0 ) ).xyz );
}
vec3 inverseTransformDirection( in vec3 dir, in mat4 matrix ) {
	return normalize( ( vec4( dir, 0.0 ) * matrix ).xyz );
}
mat3 transposeMat3( const in mat3 m ) {
	mat3 tmp;
	tmp[ 0 ] = vec3( m[ 0 ].x, m[ 1 ].x, m[ 2 ].x );
	tmp[ 1 ] = vec3( m[ 0 ].y, m[ 1 ].y, m[ 2 ].y );
	tmp[ 2 ] = vec3( m[ 0 ].z, m[ 1 ].z, m[ 2 ].z );
	return tmp;
}
bool isPerspectiveMatrix( mat4 m ) {
	return m[ 2 ][ 3 ] == - 1.0;
}
vec2 equirectUv( in vec3 dir ) {
	float u = atan( dir.z, dir.x ) * RECIPROCAL_PI2 + 0.5;
	float v = asin( clamp( dir.y, - 1.0, 1.0 ) ) * RECIPROCAL_PI + 0.5;
	return vec2( u, v );
}
vec3 BRDF_Lambert( const in vec3 diffuseColor ) {
	return RECIPROCAL_PI * diffuseColor;
}
vec3 F_Schlick( const in vec3 f0, const in float f90, const in float dotVH ) {
	float fresnel = exp2( ( - 5.55473 * dotVH - 6.98316 ) * dotVH );
	return f0 * ( 1.0 - fresnel ) + ( f90 * fresnel );
}
float F_Schlick( const in float f0, const in float f90, const in float dotVH ) {
	float fresnel = exp2( ( - 5.55473 * dotVH - 6.98316 ) * dotVH );
	return f0 * ( 1.0 - fresnel ) + ( f90 * fresnel );
} // validated`,gM=`#ifdef ENVMAP_TYPE_CUBE_UV
	#define cubeUV_minMipLevel 4.0
	#define cubeUV_minTileSize 16.0
	float getFace( vec3 direction ) {
		vec3 absDirection = abs( direction );
		float face = - 1.0;
		if ( absDirection.x > absDirection.z ) {
			if ( absDirection.x > absDirection.y )
				face = direction.x > 0.0 ? 0.0 : 3.0;
			else
				face = direction.y > 0.0 ? 1.0 : 4.0;
		} else {
			if ( absDirection.z > absDirection.y )
				face = direction.z > 0.0 ? 2.0 : 5.0;
			else
				face = direction.y > 0.0 ? 1.0 : 4.0;
		}
		return face;
	}
	vec2 getUV( vec3 direction, float face ) {
		vec2 uv;
		if ( face == 0.0 ) {
			uv = vec2( direction.z, direction.y ) / abs( direction.x );
		} else if ( face == 1.0 ) {
			uv = vec2( - direction.x, - direction.z ) / abs( direction.y );
		} else if ( face == 2.0 ) {
			uv = vec2( - direction.x, direction.y ) / abs( direction.z );
		} else if ( face == 3.0 ) {
			uv = vec2( - direction.z, direction.y ) / abs( direction.x );
		} else if ( face == 4.0 ) {
			uv = vec2( - direction.x, direction.z ) / abs( direction.y );
		} else {
			uv = vec2( direction.x, direction.y ) / abs( direction.z );
		}
		return 0.5 * ( uv + 1.0 );
	}
	vec3 bilinearCubeUV( sampler2D envMap, vec3 direction, float mipInt ) {
		float face = getFace( direction );
		float filterInt = max( cubeUV_minMipLevel - mipInt, 0.0 );
		mipInt = max( mipInt, cubeUV_minMipLevel );
		float faceSize = exp2( mipInt );
		highp vec2 uv = getUV( direction, face ) * ( faceSize - 2.0 ) + 1.0;
		if ( face > 2.0 ) {
			uv.y += faceSize;
			face -= 3.0;
		}
		uv.x += face * faceSize;
		uv.x += filterInt * 3.0 * cubeUV_minTileSize;
		uv.y += 4.0 * ( exp2( CUBEUV_MAX_MIP ) - faceSize );
		uv.x *= CUBEUV_TEXEL_WIDTH;
		uv.y *= CUBEUV_TEXEL_HEIGHT;
		#ifdef texture2DGradEXT
			return texture2DGradEXT( envMap, uv, vec2( 0.0 ), vec2( 0.0 ) ).rgb;
		#else
			return texture2D( envMap, uv ).rgb;
		#endif
	}
	#define cubeUV_r0 1.0
	#define cubeUV_m0 - 2.0
	#define cubeUV_r1 0.8
	#define cubeUV_m1 - 1.0
	#define cubeUV_r4 0.4
	#define cubeUV_m4 2.0
	#define cubeUV_r5 0.305
	#define cubeUV_m5 3.0
	#define cubeUV_r6 0.21
	#define cubeUV_m6 4.0
	float roughnessToMip( float roughness ) {
		float mip = 0.0;
		if ( roughness >= cubeUV_r1 ) {
			mip = ( cubeUV_r0 - roughness ) * ( cubeUV_m1 - cubeUV_m0 ) / ( cubeUV_r0 - cubeUV_r1 ) + cubeUV_m0;
		} else if ( roughness >= cubeUV_r4 ) {
			mip = ( cubeUV_r1 - roughness ) * ( cubeUV_m4 - cubeUV_m1 ) / ( cubeUV_r1 - cubeUV_r4 ) + cubeUV_m1;
		} else if ( roughness >= cubeUV_r5 ) {
			mip = ( cubeUV_r4 - roughness ) * ( cubeUV_m5 - cubeUV_m4 ) / ( cubeUV_r4 - cubeUV_r5 ) + cubeUV_m4;
		} else if ( roughness >= cubeUV_r6 ) {
			mip = ( cubeUV_r5 - roughness ) * ( cubeUV_m6 - cubeUV_m5 ) / ( cubeUV_r5 - cubeUV_r6 ) + cubeUV_m5;
		} else {
			mip = - 2.0 * log2( 1.16 * roughness );		}
		return mip;
	}
	vec4 textureCubeUV( sampler2D envMap, vec3 sampleDir, float roughness ) {
		float mip = clamp( roughnessToMip( roughness ), cubeUV_m0, CUBEUV_MAX_MIP );
		float mipF = fract( mip );
		float mipInt = floor( mip );
		vec3 color0 = bilinearCubeUV( envMap, sampleDir, mipInt );
		if ( mipF == 0.0 ) {
			return vec4( color0, 1.0 );
		} else {
			vec3 color1 = bilinearCubeUV( envMap, sampleDir, mipInt + 1.0 );
			return vec4( mix( color0, color1, mipF ), 1.0 );
		}
	}
#endif`,_M=`vec3 transformedNormal = objectNormal;
#ifdef USE_TANGENT
	vec3 transformedTangent = objectTangent;
#endif
#ifdef USE_BATCHING
	mat3 bm = mat3( batchingMatrix );
	transformedNormal /= vec3( dot( bm[ 0 ], bm[ 0 ] ), dot( bm[ 1 ], bm[ 1 ] ), dot( bm[ 2 ], bm[ 2 ] ) );
	transformedNormal = bm * transformedNormal;
	#ifdef USE_TANGENT
		transformedTangent = bm * transformedTangent;
	#endif
#endif
#ifdef USE_INSTANCING
	mat3 im = mat3( instanceMatrix );
	transformedNormal /= vec3( dot( im[ 0 ], im[ 0 ] ), dot( im[ 1 ], im[ 1 ] ), dot( im[ 2 ], im[ 2 ] ) );
	transformedNormal = im * transformedNormal;
	#ifdef USE_TANGENT
		transformedTangent = im * transformedTangent;
	#endif
#endif
transformedNormal = normalMatrix * transformedNormal;
#ifdef FLIP_SIDED
	transformedNormal = - transformedNormal;
#endif
#ifdef USE_TANGENT
	transformedTangent = ( modelViewMatrix * vec4( transformedTangent, 0.0 ) ).xyz;
	#ifdef FLIP_SIDED
		transformedTangent = - transformedTangent;
	#endif
#endif`,vM=`#ifdef USE_DISPLACEMENTMAP
	uniform sampler2D displacementMap;
	uniform float displacementScale;
	uniform float displacementBias;
#endif`,yM=`#ifdef USE_DISPLACEMENTMAP
	transformed += normalize( objectNormal ) * ( texture2D( displacementMap, vDisplacementMapUv ).x * displacementScale + displacementBias );
#endif`,xM=`#ifdef USE_EMISSIVEMAP
	vec4 emissiveColor = texture2D( emissiveMap, vEmissiveMapUv );
	totalEmissiveRadiance *= emissiveColor.rgb;
#endif`,SM=`#ifdef USE_EMISSIVEMAP
	uniform sampler2D emissiveMap;
#endif`,MM="gl_FragColor = linearToOutputTexel( gl_FragColor );",EM=`
const mat3 LINEAR_SRGB_TO_LINEAR_DISPLAY_P3 = mat3(
	vec3( 0.8224621, 0.177538, 0.0 ),
	vec3( 0.0331941, 0.9668058, 0.0 ),
	vec3( 0.0170827, 0.0723974, 0.9105199 )
);
const mat3 LINEAR_DISPLAY_P3_TO_LINEAR_SRGB = mat3(
	vec3( 1.2249401, - 0.2249404, 0.0 ),
	vec3( - 0.0420569, 1.0420571, 0.0 ),
	vec3( - 0.0196376, - 0.0786361, 1.0982735 )
);
vec4 LinearSRGBToLinearDisplayP3( in vec4 value ) {
	return vec4( value.rgb * LINEAR_SRGB_TO_LINEAR_DISPLAY_P3, value.a );
}
vec4 LinearDisplayP3ToLinearSRGB( in vec4 value ) {
	return vec4( value.rgb * LINEAR_DISPLAY_P3_TO_LINEAR_SRGB, value.a );
}
vec4 LinearTransferOETF( in vec4 value ) {
	return value;
}
vec4 sRGBTransferOETF( in vec4 value ) {
	return vec4( mix( pow( value.rgb, vec3( 0.41666 ) ) * 1.055 - vec3( 0.055 ), value.rgb * 12.92, vec3( lessThanEqual( value.rgb, vec3( 0.0031308 ) ) ) ), value.a );
}`,wM=`#ifdef USE_ENVMAP
	#ifdef ENV_WORLDPOS
		vec3 cameraToFrag;
		if ( isOrthographic ) {
			cameraToFrag = normalize( vec3( - viewMatrix[ 0 ][ 2 ], - viewMatrix[ 1 ][ 2 ], - viewMatrix[ 2 ][ 2 ] ) );
		} else {
			cameraToFrag = normalize( vWorldPosition - cameraPosition );
		}
		vec3 worldNormal = inverseTransformDirection( normal, viewMatrix );
		#ifdef ENVMAP_MODE_REFLECTION
			vec3 reflectVec = reflect( cameraToFrag, worldNormal );
		#else
			vec3 reflectVec = refract( cameraToFrag, worldNormal, refractionRatio );
		#endif
	#else
		vec3 reflectVec = vReflect;
	#endif
	#ifdef ENVMAP_TYPE_CUBE
		vec4 envColor = textureCube( envMap, envMapRotation * vec3( flipEnvMap * reflectVec.x, reflectVec.yz ) );
	#else
		vec4 envColor = vec4( 0.0 );
	#endif
	#ifdef ENVMAP_BLENDING_MULTIPLY
		outgoingLight = mix( outgoingLight, outgoingLight * envColor.xyz, specularStrength * reflectivity );
	#elif defined( ENVMAP_BLENDING_MIX )
		outgoingLight = mix( outgoingLight, envColor.xyz, specularStrength * reflectivity );
	#elif defined( ENVMAP_BLENDING_ADD )
		outgoingLight += envColor.xyz * specularStrength * reflectivity;
	#endif
#endif`,TM=`#ifdef USE_ENVMAP
	uniform float envMapIntensity;
	uniform float flipEnvMap;
	uniform mat3 envMapRotation;
	#ifdef ENVMAP_TYPE_CUBE
		uniform samplerCube envMap;
	#else
		uniform sampler2D envMap;
	#endif
	
#endif`,AM=`#ifdef USE_ENVMAP
	uniform float reflectivity;
	#if defined( USE_BUMPMAP ) || defined( USE_NORMALMAP ) || defined( PHONG ) || defined( LAMBERT )
		#define ENV_WORLDPOS
	#endif
	#ifdef ENV_WORLDPOS
		varying vec3 vWorldPosition;
		uniform float refractionRatio;
	#else
		varying vec3 vReflect;
	#endif
#endif`,CM=`#ifdef USE_ENVMAP
	#if defined( USE_BUMPMAP ) || defined( USE_NORMALMAP ) || defined( PHONG ) || defined( LAMBERT )
		#define ENV_WORLDPOS
	#endif
	#ifdef ENV_WORLDPOS
		
		varying vec3 vWorldPosition;
	#else
		varying vec3 vReflect;
		uniform float refractionRatio;
	#endif
#endif`,RM=`#ifdef USE_ENVMAP
	#ifdef ENV_WORLDPOS
		vWorldPosition = worldPosition.xyz;
	#else
		vec3 cameraToVertex;
		if ( isOrthographic ) {
			cameraToVertex = normalize( vec3( - viewMatrix[ 0 ][ 2 ], - viewMatrix[ 1 ][ 2 ], - viewMatrix[ 2 ][ 2 ] ) );
		} else {
			cameraToVertex = normalize( worldPosition.xyz - cameraPosition );
		}
		vec3 worldNormal = inverseTransformDirection( transformedNormal, viewMatrix );
		#ifdef ENVMAP_MODE_REFLECTION
			vReflect = reflect( cameraToVertex, worldNormal );
		#else
			vReflect = refract( cameraToVertex, worldNormal, refractionRatio );
		#endif
	#endif
#endif`,PM=`#ifdef USE_FOG
	vFogDepth = - mvPosition.z;
#endif`,bM=`#ifdef USE_FOG
	varying float vFogDepth;
#endif`,LM=`#ifdef USE_FOG
	#ifdef FOG_EXP2
		float fogFactor = 1.0 - exp( - fogDensity * fogDensity * vFogDepth * vFogDepth );
	#else
		float fogFactor = smoothstep( fogNear, fogFar, vFogDepth );
	#endif
	gl_FragColor.rgb = mix( gl_FragColor.rgb, fogColor, fogFactor );
#endif`,DM=`#ifdef USE_FOG
	uniform vec3 fogColor;
	varying float vFogDepth;
	#ifdef FOG_EXP2
		uniform float fogDensity;
	#else
		uniform float fogNear;
		uniform float fogFar;
	#endif
#endif`,NM=`#ifdef USE_GRADIENTMAP
	uniform sampler2D gradientMap;
#endif
vec3 getGradientIrradiance( vec3 normal, vec3 lightDirection ) {
	float dotNL = dot( normal, lightDirection );
	vec2 coord = vec2( dotNL * 0.5 + 0.5, 0.0 );
	#ifdef USE_GRADIENTMAP
		return vec3( texture2D( gradientMap, coord ).r );
	#else
		vec2 fw = fwidth( coord ) * 0.5;
		return mix( vec3( 0.7 ), vec3( 1.0 ), smoothstep( 0.7 - fw.x, 0.7 + fw.x, coord.x ) );
	#endif
}`,IM=`#ifdef USE_LIGHTMAP
	uniform sampler2D lightMap;
	uniform float lightMapIntensity;
#endif`,UM=`LambertMaterial material;
material.diffuseColor = diffuseColor.rgb;
material.specularStrength = specularStrength;`,FM=`varying vec3 vViewPosition;
struct LambertMaterial {
	vec3 diffuseColor;
	float specularStrength;
};
void RE_Direct_Lambert( const in IncidentLight directLight, const in vec3 geometryPosition, const in vec3 geometryNormal, const in vec3 geometryViewDir, const in vec3 geometryClearcoatNormal, const in LambertMaterial material, inout ReflectedLight reflectedLight ) {
	float dotNL = saturate( dot( geometryNormal, directLight.direction ) );
	vec3 irradiance = dotNL * directLight.color;
	reflectedLight.directDiffuse += irradiance * BRDF_Lambert( material.diffuseColor );
}
void RE_IndirectDiffuse_Lambert( const in vec3 irradiance, const in vec3 geometryPosition, const in vec3 geometryNormal, const in vec3 geometryViewDir, const in vec3 geometryClearcoatNormal, const in LambertMaterial material, inout ReflectedLight reflectedLight ) {
	reflectedLight.indirectDiffuse += irradiance * BRDF_Lambert( material.diffuseColor );
}
#define RE_Direct				RE_Direct_Lambert
#define RE_IndirectDiffuse		RE_IndirectDiffuse_Lambert`,OM=`uniform bool receiveShadow;
uniform vec3 ambientLightColor;
#if defined( USE_LIGHT_PROBES )
	uniform vec3 lightProbe[ 9 ];
#endif
vec3 shGetIrradianceAt( in vec3 normal, in vec3 shCoefficients[ 9 ] ) {
	float x = normal.x, y = normal.y, z = normal.z;
	vec3 result = shCoefficients[ 0 ] * 0.886227;
	result += shCoefficients[ 1 ] * 2.0 * 0.511664 * y;
	result += shCoefficients[ 2 ] * 2.0 * 0.511664 * z;
	result += shCoefficients[ 3 ] * 2.0 * 0.511664 * x;
	result += shCoefficients[ 4 ] * 2.0 * 0.429043 * x * y;
	result += shCoefficients[ 5 ] * 2.0 * 0.429043 * y * z;
	result += shCoefficients[ 6 ] * ( 0.743125 * z * z - 0.247708 );
	result += shCoefficients[ 7 ] * 2.0 * 0.429043 * x * z;
	result += shCoefficients[ 8 ] * 0.429043 * ( x * x - y * y );
	return result;
}
vec3 getLightProbeIrradiance( const in vec3 lightProbe[ 9 ], const in vec3 normal ) {
	vec3 worldNormal = inverseTransformDirection( normal, viewMatrix );
	vec3 irradiance = shGetIrradianceAt( worldNormal, lightProbe );
	return irradiance;
}
vec3 getAmbientLightIrradiance( const in vec3 ambientLightColor ) {
	vec3 irradiance = ambientLightColor;
	return irradiance;
}
float getDistanceAttenuation( const in float lightDistance, const in float cutoffDistance, const in float decayExponent ) {
	float distanceFalloff = 1.0 / max( pow( lightDistance, decayExponent ), 0.01 );
	if ( cutoffDistance > 0.0 ) {
		distanceFalloff *= pow2( saturate( 1.0 - pow4( lightDistance / cutoffDistance ) ) );
	}
	return distanceFalloff;
}
float getSpotAttenuation( const in float coneCosine, const in float penumbraCosine, const in float angleCosine ) {
	return smoothstep( coneCosine, penumbraCosine, angleCosine );
}
#if NUM_DIR_LIGHTS > 0
	struct DirectionalLight {
		vec3 direction;
		vec3 color;
	};
	uniform DirectionalLight directionalLights[ NUM_DIR_LIGHTS ];
	void getDirectionalLightInfo( const in DirectionalLight directionalLight, out IncidentLight light ) {
		light.color = directionalLight.color;
		light.direction = directionalLight.direction;
		light.visible = true;
	}
#endif
#if NUM_POINT_LIGHTS > 0
	struct PointLight {
		vec3 position;
		vec3 color;
		float distance;
		float decay;
	};
	uniform PointLight pointLights[ NUM_POINT_LIGHTS ];
	void getPointLightInfo( const in PointLight pointLight, const in vec3 geometryPosition, out IncidentLight light ) {
		vec3 lVector = pointLight.position - geometryPosition;
		light.direction = normalize( lVector );
		float lightDistance = length( lVector );
		light.color = pointLight.color;
		light.color *= getDistanceAttenuation( lightDistance, pointLight.distance, pointLight.decay );
		light.visible = ( light.color != vec3( 0.0 ) );
	}
#endif
#if NUM_SPOT_LIGHTS > 0
	struct SpotLight {
		vec3 position;
		vec3 direction;
		vec3 color;
		float distance;
		float decay;
		float coneCos;
		float penumbraCos;
	};
	uniform SpotLight spotLights[ NUM_SPOT_LIGHTS ];
	void getSpotLightInfo( const in SpotLight spotLight, const in vec3 geometryPosition, out IncidentLight light ) {
		vec3 lVector = spotLight.position - geometryPosition;
		light.direction = normalize( lVector );
		float angleCos = dot( light.direction, spotLight.direction );
		float spotAttenuation = getSpotAttenuation( spotLight.coneCos, spotLight.penumbraCos, angleCos );
		if ( spotAttenuation > 0.0 ) {
			float lightDistance = length( lVector );
			light.color = spotLight.color * spotAttenuation;
			light.color *= getDistanceAttenuation( lightDistance, spotLight.distance, spotLight.decay );
			light.visible = ( light.color != vec3( 0.0 ) );
		} else {
			light.color = vec3( 0.0 );
			light.visible = false;
		}
	}
#endif
#if NUM_RECT_AREA_LIGHTS > 0
	struct RectAreaLight {
		vec3 color;
		vec3 position;
		vec3 halfWidth;
		vec3 halfHeight;
	};
	uniform sampler2D ltc_1;	uniform sampler2D ltc_2;
	uniform RectAreaLight rectAreaLights[ NUM_RECT_AREA_LIGHTS ];
#endif
#if NUM_HEMI_LIGHTS > 0
	struct HemisphereLight {
		vec3 direction;
		vec3 skyColor;
		vec3 groundColor;
	};
	uniform HemisphereLight hemisphereLights[ NUM_HEMI_LIGHTS ];
	vec3 getHemisphereLightIrradiance( const in HemisphereLight hemiLight, const in vec3 normal ) {
		float dotNL = dot( normal, hemiLight.direction );
		float hemiDiffuseWeight = 0.5 * dotNL + 0.5;
		vec3 irradiance = mix( hemiLight.groundColor, hemiLight.skyColor, hemiDiffuseWeight );
		return irradiance;
	}
#endif`,BM=`#ifdef USE_ENVMAP
	vec3 getIBLIrradiance( const in vec3 normal ) {
		#ifdef ENVMAP_TYPE_CUBE_UV
			vec3 worldNormal = inverseTransformDirection( normal, viewMatrix );
			vec4 envMapColor = textureCubeUV( envMap, envMapRotation * worldNormal, 1.0 );
			return PI * envMapColor.rgb * envMapIntensity;
		#else
			return vec3( 0.0 );
		#endif
	}
	vec3 getIBLRadiance( const in vec3 viewDir, const in vec3 normal, const in float roughness ) {
		#ifdef ENVMAP_TYPE_CUBE_UV
			vec3 reflectVec = reflect( - viewDir, normal );
			reflectVec = normalize( mix( reflectVec, normal, roughness * roughness) );
			reflectVec = inverseTransformDirection( reflectVec, viewMatrix );
			vec4 envMapColor = textureCubeUV( envMap, envMapRotation * reflectVec, roughness );
			return envMapColor.rgb * envMapIntensity;
		#else
			return vec3( 0.0 );
		#endif
	}
	#ifdef USE_ANISOTROPY
		vec3 getIBLAnisotropyRadiance( const in vec3 viewDir, const in vec3 normal, const in float roughness, const in vec3 bitangent, const in float anisotropy ) {
			#ifdef ENVMAP_TYPE_CUBE_UV
				vec3 bentNormal = cross( bitangent, viewDir );
				bentNormal = normalize( cross( bentNormal, bitangent ) );
				bentNormal = normalize( mix( bentNormal, normal, pow2( pow2( 1.0 - anisotropy * ( 1.0 - roughness ) ) ) ) );
				return getIBLRadiance( viewDir, bentNormal, roughness );
			#else
				return vec3( 0.0 );
			#endif
		}
	#endif
#endif`,zM=`ToonMaterial material;
material.diffuseColor = diffuseColor.rgb;`,kM=`varying vec3 vViewPosition;
struct ToonMaterial {
	vec3 diffuseColor;
};
void RE_Direct_Toon( const in IncidentLight directLight, const in vec3 geometryPosition, const in vec3 geometryNormal, const in vec3 geometryViewDir, const in vec3 geometryClearcoatNormal, const in ToonMaterial material, inout ReflectedLight reflectedLight ) {
	vec3 irradiance = getGradientIrradiance( geometryNormal, directLight.direction ) * directLight.color;
	reflectedLight.directDiffuse += irradiance * BRDF_Lambert( material.diffuseColor );
}
void RE_IndirectDiffuse_Toon( const in vec3 irradiance, const in vec3 geometryPosition, const in vec3 geometryNormal, const in vec3 geometryViewDir, const in vec3 geometryClearcoatNormal, const in ToonMaterial material, inout ReflectedLight reflectedLight ) {
	reflectedLight.indirectDiffuse += irradiance * BRDF_Lambert( material.diffuseColor );
}
#define RE_Direct				RE_Direct_Toon
#define RE_IndirectDiffuse		RE_IndirectDiffuse_Toon`,HM=`BlinnPhongMaterial material;
material.diffuseColor = diffuseColor.rgb;
material.specularColor = specular;
material.specularShininess = shininess;
material.specularStrength = specularStrength;`,VM=`varying vec3 vViewPosition;
struct BlinnPhongMaterial {
	vec3 diffuseColor;
	vec3 specularColor;
	float specularShininess;
	float specularStrength;
};
void RE_Direct_BlinnPhong( const in IncidentLight directLight, const in vec3 geometryPosition, const in vec3 geometryNormal, const in vec3 geometryViewDir, const in vec3 geometryClearcoatNormal, const in BlinnPhongMaterial material, inout ReflectedLight reflectedLight ) {
	float dotNL = saturate( dot( geometryNormal, directLight.direction ) );
	vec3 irradiance = dotNL * directLight.color;
	reflectedLight.directDiffuse += irradiance * BRDF_Lambert( material.diffuseColor );
	reflectedLight.directSpecular += irradiance * BRDF_BlinnPhong( directLight.direction, geometryViewDir, geometryNormal, material.specularColor, material.specularShininess ) * material.specularStrength;
}
void RE_IndirectDiffuse_BlinnPhong( const in vec3 irradiance, const in vec3 geometryPosition, const in vec3 geometryNormal, const in vec3 geometryViewDir, const in vec3 geometryClearcoatNormal, const in BlinnPhongMaterial material, inout ReflectedLight reflectedLight ) {
	reflectedLight.indirectDiffuse += irradiance * BRDF_Lambert( material.diffuseColor );
}
#define RE_Direct				RE_Direct_BlinnPhong
#define RE_IndirectDiffuse		RE_IndirectDiffuse_BlinnPhong`,GM=`PhysicalMaterial material;
material.diffuseColor = diffuseColor.rgb * ( 1.0 - metalnessFactor );
vec3 dxy = max( abs( dFdx( nonPerturbedNormal ) ), abs( dFdy( nonPerturbedNormal ) ) );
float geometryRoughness = max( max( dxy.x, dxy.y ), dxy.z );
material.roughness = max( roughnessFactor, 0.0525 );material.roughness += geometryRoughness;
material.roughness = min( material.roughness, 1.0 );
#ifdef IOR
	material.ior = ior;
	#ifdef USE_SPECULAR
		float specularIntensityFactor = specularIntensity;
		vec3 specularColorFactor = specularColor;
		#ifdef USE_SPECULAR_COLORMAP
			specularColorFactor *= texture2D( specularColorMap, vSpecularColorMapUv ).rgb;
		#endif
		#ifdef USE_SPECULAR_INTENSITYMAP
			specularIntensityFactor *= texture2D( specularIntensityMap, vSpecularIntensityMapUv ).a;
		#endif
		material.specularF90 = mix( specularIntensityFactor, 1.0, metalnessFactor );
	#else
		float specularIntensityFactor = 1.0;
		vec3 specularColorFactor = vec3( 1.0 );
		material.specularF90 = 1.0;
	#endif
	material.specularColor = mix( min( pow2( ( material.ior - 1.0 ) / ( material.ior + 1.0 ) ) * specularColorFactor, vec3( 1.0 ) ) * specularIntensityFactor, diffuseColor.rgb, metalnessFactor );
#else
	material.specularColor = mix( vec3( 0.04 ), diffuseColor.rgb, metalnessFactor );
	material.specularF90 = 1.0;
#endif
#ifdef USE_CLEARCOAT
	material.clearcoat = clearcoat;
	material.clearcoatRoughness = clearcoatRoughness;
	material.clearcoatF0 = vec3( 0.04 );
	material.clearcoatF90 = 1.0;
	#ifdef USE_CLEARCOATMAP
		material.clearcoat *= texture2D( clearcoatMap, vClearcoatMapUv ).x;
	#endif
	#ifdef USE_CLEARCOAT_ROUGHNESSMAP
		material.clearcoatRoughness *= texture2D( clearcoatRoughnessMap, vClearcoatRoughnessMapUv ).y;
	#endif
	material.clearcoat = saturate( material.clearcoat );	material.clearcoatRoughness = max( material.clearcoatRoughness, 0.0525 );
	material.clearcoatRoughness += geometryRoughness;
	material.clearcoatRoughness = min( material.clearcoatRoughness, 1.0 );
#endif
#ifdef USE_DISPERSION
	material.dispersion = dispersion;
#endif
#ifdef USE_IRIDESCENCE
	material.iridescence = iridescence;
	material.iridescenceIOR = iridescenceIOR;
	#ifdef USE_IRIDESCENCEMAP
		material.iridescence *= texture2D( iridescenceMap, vIridescenceMapUv ).r;
	#endif
	#ifdef USE_IRIDESCENCE_THICKNESSMAP
		material.iridescenceThickness = (iridescenceThicknessMaximum - iridescenceThicknessMinimum) * texture2D( iridescenceThicknessMap, vIridescenceThicknessMapUv ).g + iridescenceThicknessMinimum;
	#else
		material.iridescenceThickness = iridescenceThicknessMaximum;
	#endif
#endif
#ifdef USE_SHEEN
	material.sheenColor = sheenColor;
	#ifdef USE_SHEEN_COLORMAP
		material.sheenColor *= texture2D( sheenColorMap, vSheenColorMapUv ).rgb;
	#endif
	material.sheenRoughness = clamp( sheenRoughness, 0.07, 1.0 );
	#ifdef USE_SHEEN_ROUGHNESSMAP
		material.sheenRoughness *= texture2D( sheenRoughnessMap, vSheenRoughnessMapUv ).a;
	#endif
#endif
#ifdef USE_ANISOTROPY
	#ifdef USE_ANISOTROPYMAP
		mat2 anisotropyMat = mat2( anisotropyVector.x, anisotropyVector.y, - anisotropyVector.y, anisotropyVector.x );
		vec3 anisotropyPolar = texture2D( anisotropyMap, vAnisotropyMapUv ).rgb;
		vec2 anisotropyV = anisotropyMat * normalize( 2.0 * anisotropyPolar.rg - vec2( 1.0 ) ) * anisotropyPolar.b;
	#else
		vec2 anisotropyV = anisotropyVector;
	#endif
	material.anisotropy = length( anisotropyV );
	if( material.anisotropy == 0.0 ) {
		anisotropyV = vec2( 1.0, 0.0 );
	} else {
		anisotropyV /= material.anisotropy;
		material.anisotropy = saturate( material.anisotropy );
	}
	material.alphaT = mix( pow2( material.roughness ), 1.0, pow2( material.anisotropy ) );
	material.anisotropyT = tbn[ 0 ] * anisotropyV.x + tbn[ 1 ] * anisotropyV.y;
	material.anisotropyB = tbn[ 1 ] * anisotropyV.x - tbn[ 0 ] * anisotropyV.y;
#endif`,WM=`struct PhysicalMaterial {
	vec3 diffuseColor;
	float roughness;
	vec3 specularColor;
	float specularF90;
	float dispersion;
	#ifdef USE_CLEARCOAT
		float clearcoat;
		float clearcoatRoughness;
		vec3 clearcoatF0;
		float clearcoatF90;
	#endif
	#ifdef USE_IRIDESCENCE
		float iridescence;
		float iridescenceIOR;
		float iridescenceThickness;
		vec3 iridescenceFresnel;
		vec3 iridescenceF0;
	#endif
	#ifdef USE_SHEEN
		vec3 sheenColor;
		float sheenRoughness;
	#endif
	#ifdef IOR
		float ior;
	#endif
	#ifdef USE_TRANSMISSION
		float transmission;
		float transmissionAlpha;
		float thickness;
		float attenuationDistance;
		vec3 attenuationColor;
	#endif
	#ifdef USE_ANISOTROPY
		float anisotropy;
		float alphaT;
		vec3 anisotropyT;
		vec3 anisotropyB;
	#endif
};
vec3 clearcoatSpecularDirect = vec3( 0.0 );
vec3 clearcoatSpecularIndirect = vec3( 0.0 );
vec3 sheenSpecularDirect = vec3( 0.0 );
vec3 sheenSpecularIndirect = vec3(0.0 );
vec3 Schlick_to_F0( const in vec3 f, const in float f90, const in float dotVH ) {
    float x = clamp( 1.0 - dotVH, 0.0, 1.0 );
    float x2 = x * x;
    float x5 = clamp( x * x2 * x2, 0.0, 0.9999 );
    return ( f - vec3( f90 ) * x5 ) / ( 1.0 - x5 );
}
float V_GGX_SmithCorrelated( const in float alpha, const in float dotNL, const in float dotNV ) {
	float a2 = pow2( alpha );
	float gv = dotNL * sqrt( a2 + ( 1.0 - a2 ) * pow2( dotNV ) );
	float gl = dotNV * sqrt( a2 + ( 1.0 - a2 ) * pow2( dotNL ) );
	return 0.5 / max( gv + gl, EPSILON );
}
float D_GGX( const in float alpha, const in float dotNH ) {
	float a2 = pow2( alpha );
	float denom = pow2( dotNH ) * ( a2 - 1.0 ) + 1.0;
	return RECIPROCAL_PI * a2 / pow2( denom );
}
#ifdef USE_ANISOTROPY
	float V_GGX_SmithCorrelated_Anisotropic( const in float alphaT, const in float alphaB, const in float dotTV, const in float dotBV, const in float dotTL, const in float dotBL, const in float dotNV, const in float dotNL ) {
		float gv = dotNL * length( vec3( alphaT * dotTV, alphaB * dotBV, dotNV ) );
		float gl = dotNV * length( vec3( alphaT * dotTL, alphaB * dotBL, dotNL ) );
		float v = 0.5 / ( gv + gl );
		return saturate(v);
	}
	float D_GGX_Anisotropic( const in float alphaT, const in float alphaB, const in float dotNH, const in float dotTH, const in float dotBH ) {
		float a2 = alphaT * alphaB;
		highp vec3 v = vec3( alphaB * dotTH, alphaT * dotBH, a2 * dotNH );
		highp float v2 = dot( v, v );
		float w2 = a2 / v2;
		return RECIPROCAL_PI * a2 * pow2 ( w2 );
	}
#endif
#ifdef USE_CLEARCOAT
	vec3 BRDF_GGX_Clearcoat( const in vec3 lightDir, const in vec3 viewDir, const in vec3 normal, const in PhysicalMaterial material) {
		vec3 f0 = material.clearcoatF0;
		float f90 = material.clearcoatF90;
		float roughness = material.clearcoatRoughness;
		float alpha = pow2( roughness );
		vec3 halfDir = normalize( lightDir + viewDir );
		float dotNL = saturate( dot( normal, lightDir ) );
		float dotNV = saturate( dot( normal, viewDir ) );
		float dotNH = saturate( dot( normal, halfDir ) );
		float dotVH = saturate( dot( viewDir, halfDir ) );
		vec3 F = F_Schlick( f0, f90, dotVH );
		float V = V_GGX_SmithCorrelated( alpha, dotNL, dotNV );
		float D = D_GGX( alpha, dotNH );
		return F * ( V * D );
	}
#endif
vec3 BRDF_GGX( const in vec3 lightDir, const in vec3 viewDir, const in vec3 normal, const in PhysicalMaterial material ) {
	vec3 f0 = material.specularColor;
	float f90 = material.specularF90;
	float roughness = material.roughness;
	float alpha = pow2( roughness );
	vec3 halfDir = normalize( lightDir + viewDir );
	float dotNL = saturate( dot( normal, lightDir ) );
	float dotNV = saturate( dot( normal, viewDir ) );
	float dotNH = saturate( dot( normal, halfDir ) );
	float dotVH = saturate( dot( viewDir, halfDir ) );
	vec3 F = F_Schlick( f0, f90, dotVH );
	#ifdef USE_IRIDESCENCE
		F = mix( F, material.iridescenceFresnel, material.iridescence );
	#endif
	#ifdef USE_ANISOTROPY
		float dotTL = dot( material.anisotropyT, lightDir );
		float dotTV = dot( material.anisotropyT, viewDir );
		float dotTH = dot( material.anisotropyT, halfDir );
		float dotBL = dot( material.anisotropyB, lightDir );
		float dotBV = dot( material.anisotropyB, viewDir );
		float dotBH = dot( material.anisotropyB, halfDir );
		float V = V_GGX_SmithCorrelated_Anisotropic( material.alphaT, alpha, dotTV, dotBV, dotTL, dotBL, dotNV, dotNL );
		float D = D_GGX_Anisotropic( material.alphaT, alpha, dotNH, dotTH, dotBH );
	#else
		float V = V_GGX_SmithCorrelated( alpha, dotNL, dotNV );
		float D = D_GGX( alpha, dotNH );
	#endif
	return F * ( V * D );
}
vec2 LTC_Uv( const in vec3 N, const in vec3 V, const in float roughness ) {
	const float LUT_SIZE = 64.0;
	const float LUT_SCALE = ( LUT_SIZE - 1.0 ) / LUT_SIZE;
	const float LUT_BIAS = 0.5 / LUT_SIZE;
	float dotNV = saturate( dot( N, V ) );
	vec2 uv = vec2( roughness, sqrt( 1.0 - dotNV ) );
	uv = uv * LUT_SCALE + LUT_BIAS;
	return uv;
}
float LTC_ClippedSphereFormFactor( const in vec3 f ) {
	float l = length( f );
	return max( ( l * l + f.z ) / ( l + 1.0 ), 0.0 );
}
vec3 LTC_EdgeVectorFormFactor( const in vec3 v1, const in vec3 v2 ) {
	float x = dot( v1, v2 );
	float y = abs( x );
	float a = 0.8543985 + ( 0.4965155 + 0.0145206 * y ) * y;
	float b = 3.4175940 + ( 4.1616724 + y ) * y;
	float v = a / b;
	float theta_sintheta = ( x > 0.0 ) ? v : 0.5 * inversesqrt( max( 1.0 - x * x, 1e-7 ) ) - v;
	return cross( v1, v2 ) * theta_sintheta;
}
vec3 LTC_Evaluate( const in vec3 N, const in vec3 V, const in vec3 P, const in mat3 mInv, const in vec3 rectCoords[ 4 ] ) {
	vec3 v1 = rectCoords[ 1 ] - rectCoords[ 0 ];
	vec3 v2 = rectCoords[ 3 ] - rectCoords[ 0 ];
	vec3 lightNormal = cross( v1, v2 );
	if( dot( lightNormal, P - rectCoords[ 0 ] ) < 0.0 ) return vec3( 0.0 );
	vec3 T1, T2;
	T1 = normalize( V - N * dot( V, N ) );
	T2 = - cross( N, T1 );
	mat3 mat = mInv * transposeMat3( mat3( T1, T2, N ) );
	vec3 coords[ 4 ];
	coords[ 0 ] = mat * ( rectCoords[ 0 ] - P );
	coords[ 1 ] = mat * ( rectCoords[ 1 ] - P );
	coords[ 2 ] = mat * ( rectCoords[ 2 ] - P );
	coords[ 3 ] = mat * ( rectCoords[ 3 ] - P );
	coords[ 0 ] = normalize( coords[ 0 ] );
	coords[ 1 ] = normalize( coords[ 1 ] );
	coords[ 2 ] = normalize( coords[ 2 ] );
	coords[ 3 ] = normalize( coords[ 3 ] );
	vec3 vectorFormFactor = vec3( 0.0 );
	vectorFormFactor += LTC_EdgeVectorFormFactor( coords[ 0 ], coords[ 1 ] );
	vectorFormFactor += LTC_EdgeVectorFormFactor( coords[ 1 ], coords[ 2 ] );
	vectorFormFactor += LTC_EdgeVectorFormFactor( coords[ 2 ], coords[ 3 ] );
	vectorFormFactor += LTC_EdgeVectorFormFactor( coords[ 3 ], coords[ 0 ] );
	float result = LTC_ClippedSphereFormFactor( vectorFormFactor );
	return vec3( result );
}
#if defined( USE_SHEEN )
float D_Charlie( float roughness, float dotNH ) {
	float alpha = pow2( roughness );
	float invAlpha = 1.0 / alpha;
	float cos2h = dotNH * dotNH;
	float sin2h = max( 1.0 - cos2h, 0.0078125 );
	return ( 2.0 + invAlpha ) * pow( sin2h, invAlpha * 0.5 ) / ( 2.0 * PI );
}
float V_Neubelt( float dotNV, float dotNL ) {
	return saturate( 1.0 / ( 4.0 * ( dotNL + dotNV - dotNL * dotNV ) ) );
}
vec3 BRDF_Sheen( const in vec3 lightDir, const in vec3 viewDir, const in vec3 normal, vec3 sheenColor, const in float sheenRoughness ) {
	vec3 halfDir = normalize( lightDir + viewDir );
	float dotNL = saturate( dot( normal, lightDir ) );
	float dotNV = saturate( dot( normal, viewDir ) );
	float dotNH = saturate( dot( normal, halfDir ) );
	float D = D_Charlie( sheenRoughness, dotNH );
	float V = V_Neubelt( dotNV, dotNL );
	return sheenColor * ( D * V );
}
#endif
float IBLSheenBRDF( const in vec3 normal, const in vec3 viewDir, const in float roughness ) {
	float dotNV = saturate( dot( normal, viewDir ) );
	float r2 = roughness * roughness;
	float a = roughness < 0.25 ? -339.2 * r2 + 161.4 * roughness - 25.9 : -8.48 * r2 + 14.3 * roughness - 9.95;
	float b = roughness < 0.25 ? 44.0 * r2 - 23.7 * roughness + 3.26 : 1.97 * r2 - 3.27 * roughness + 0.72;
	float DG = exp( a * dotNV + b ) + ( roughness < 0.25 ? 0.0 : 0.1 * ( roughness - 0.25 ) );
	return saturate( DG * RECIPROCAL_PI );
}
vec2 DFGApprox( const in vec3 normal, const in vec3 viewDir, const in float roughness ) {
	float dotNV = saturate( dot( normal, viewDir ) );
	const vec4 c0 = vec4( - 1, - 0.0275, - 0.572, 0.022 );
	const vec4 c1 = vec4( 1, 0.0425, 1.04, - 0.04 );
	vec4 r = roughness * c0 + c1;
	float a004 = min( r.x * r.x, exp2( - 9.28 * dotNV ) ) * r.x + r.y;
	vec2 fab = vec2( - 1.04, 1.04 ) * a004 + r.zw;
	return fab;
}
vec3 EnvironmentBRDF( const in vec3 normal, const in vec3 viewDir, const in vec3 specularColor, const in float specularF90, const in float roughness ) {
	vec2 fab = DFGApprox( normal, viewDir, roughness );
	return specularColor * fab.x + specularF90 * fab.y;
}
#ifdef USE_IRIDESCENCE
void computeMultiscatteringIridescence( const in vec3 normal, const in vec3 viewDir, const in vec3 specularColor, const in float specularF90, const in float iridescence, const in vec3 iridescenceF0, const in float roughness, inout vec3 singleScatter, inout vec3 multiScatter ) {
#else
void computeMultiscattering( const in vec3 normal, const in vec3 viewDir, const in vec3 specularColor, const in float specularF90, const in float roughness, inout vec3 singleScatter, inout vec3 multiScatter ) {
#endif
	vec2 fab = DFGApprox( normal, viewDir, roughness );
	#ifdef USE_IRIDESCENCE
		vec3 Fr = mix( specularColor, iridescenceF0, iridescence );
	#else
		vec3 Fr = specularColor;
	#endif
	vec3 FssEss = Fr * fab.x + specularF90 * fab.y;
	float Ess = fab.x + fab.y;
	float Ems = 1.0 - Ess;
	vec3 Favg = Fr + ( 1.0 - Fr ) * 0.047619;	vec3 Fms = FssEss * Favg / ( 1.0 - Ems * Favg );
	singleScatter += FssEss;
	multiScatter += Fms * Ems;
}
#if NUM_RECT_AREA_LIGHTS > 0
	void RE_Direct_RectArea_Physical( const in RectAreaLight rectAreaLight, const in vec3 geometryPosition, const in vec3 geometryNormal, const in vec3 geometryViewDir, const in vec3 geometryClearcoatNormal, const in PhysicalMaterial material, inout ReflectedLight reflectedLight ) {
		vec3 normal = geometryNormal;
		vec3 viewDir = geometryViewDir;
		vec3 position = geometryPosition;
		vec3 lightPos = rectAreaLight.position;
		vec3 halfWidth = rectAreaLight.halfWidth;
		vec3 halfHeight = rectAreaLight.halfHeight;
		vec3 lightColor = rectAreaLight.color;
		float roughness = material.roughness;
		vec3 rectCoords[ 4 ];
		rectCoords[ 0 ] = lightPos + halfWidth - halfHeight;		rectCoords[ 1 ] = lightPos - halfWidth - halfHeight;
		rectCoords[ 2 ] = lightPos - halfWidth + halfHeight;
		rectCoords[ 3 ] = lightPos + halfWidth + halfHeight;
		vec2 uv = LTC_Uv( normal, viewDir, roughness );
		vec4 t1 = texture2D( ltc_1, uv );
		vec4 t2 = texture2D( ltc_2, uv );
		mat3 mInv = mat3(
			vec3( t1.x, 0, t1.y ),
			vec3(    0, 1,    0 ),
			vec3( t1.z, 0, t1.w )
		);
		vec3 fresnel = ( material.specularColor * t2.x + ( vec3( 1.0 ) - material.specularColor ) * t2.y );
		reflectedLight.directSpecular += lightColor * fresnel * LTC_Evaluate( normal, viewDir, position, mInv, rectCoords );
		reflectedLight.directDiffuse += lightColor * material.diffuseColor * LTC_Evaluate( normal, viewDir, position, mat3( 1.0 ), rectCoords );
	}
#endif
void RE_Direct_Physical( const in IncidentLight directLight, const in vec3 geometryPosition, const in vec3 geometryNormal, const in vec3 geometryViewDir, const in vec3 geometryClearcoatNormal, const in PhysicalMaterial material, inout ReflectedLight reflectedLight ) {
	float dotNL = saturate( dot( geometryNormal, directLight.direction ) );
	vec3 irradiance = dotNL * directLight.color;
	#ifdef USE_CLEARCOAT
		float dotNLcc = saturate( dot( geometryClearcoatNormal, directLight.direction ) );
		vec3 ccIrradiance = dotNLcc * directLight.color;
		clearcoatSpecularDirect += ccIrradiance * BRDF_GGX_Clearcoat( directLight.direction, geometryViewDir, geometryClearcoatNormal, material );
	#endif
	#ifdef USE_SHEEN
		sheenSpecularDirect += irradiance * BRDF_Sheen( directLight.direction, geometryViewDir, geometryNormal, material.sheenColor, material.sheenRoughness );
	#endif
	reflectedLight.directSpecular += irradiance * BRDF_GGX( directLight.direction, geometryViewDir, geometryNormal, material );
	reflectedLight.directDiffuse += irradiance * BRDF_Lambert( material.diffuseColor );
}
void RE_IndirectDiffuse_Physical( const in vec3 irradiance, const in vec3 geometryPosition, const in vec3 geometryNormal, const in vec3 geometryViewDir, const in vec3 geometryClearcoatNormal, const in PhysicalMaterial material, inout ReflectedLight reflectedLight ) {
	reflectedLight.indirectDiffuse += irradiance * BRDF_Lambert( material.diffuseColor );
}
void RE_IndirectSpecular_Physical( const in vec3 radiance, const in vec3 irradiance, const in vec3 clearcoatRadiance, const in vec3 geometryPosition, const in vec3 geometryNormal, const in vec3 geometryViewDir, const in vec3 geometryClearcoatNormal, const in PhysicalMaterial material, inout ReflectedLight reflectedLight) {
	#ifdef USE_CLEARCOAT
		clearcoatSpecularIndirect += clearcoatRadiance * EnvironmentBRDF( geometryClearcoatNormal, geometryViewDir, material.clearcoatF0, material.clearcoatF90, material.clearcoatRoughness );
	#endif
	#ifdef USE_SHEEN
		sheenSpecularIndirect += irradiance * material.sheenColor * IBLSheenBRDF( geometryNormal, geometryViewDir, material.sheenRoughness );
	#endif
	vec3 singleScattering = vec3( 0.0 );
	vec3 multiScattering = vec3( 0.0 );
	vec3 cosineWeightedIrradiance = irradiance * RECIPROCAL_PI;
	#ifdef USE_IRIDESCENCE
		computeMultiscatteringIridescence( geometryNormal, geometryViewDir, material.specularColor, material.specularF90, material.iridescence, material.iridescenceFresnel, material.roughness, singleScattering, multiScattering );
	#else
		computeMultiscattering( geometryNormal, geometryViewDir, material.specularColor, material.specularF90, material.roughness, singleScattering, multiScattering );
	#endif
	vec3 totalScattering = singleScattering + multiScattering;
	vec3 diffuse = material.diffuseColor * ( 1.0 - max( max( totalScattering.r, totalScattering.g ), totalScattering.b ) );
	reflectedLight.indirectSpecular += radiance * singleScattering;
	reflectedLight.indirectSpecular += multiScattering * cosineWeightedIrradiance;
	reflectedLight.indirectDiffuse += diffuse * cosineWeightedIrradiance;
}
#define RE_Direct				RE_Direct_Physical
#define RE_Direct_RectArea		RE_Direct_RectArea_Physical
#define RE_IndirectDiffuse		RE_IndirectDiffuse_Physical
#define RE_IndirectSpecular		RE_IndirectSpecular_Physical
float computeSpecularOcclusion( const in float dotNV, const in float ambientOcclusion, const in float roughness ) {
	return saturate( pow( dotNV + ambientOcclusion, exp2( - 16.0 * roughness - 1.0 ) ) - 1.0 + ambientOcclusion );
}`,XM=`
vec3 geometryPosition = - vViewPosition;
vec3 geometryNormal = normal;
vec3 geometryViewDir = ( isOrthographic ) ? vec3( 0, 0, 1 ) : normalize( vViewPosition );
vec3 geometryClearcoatNormal = vec3( 0.0 );
#ifdef USE_CLEARCOAT
	geometryClearcoatNormal = clearcoatNormal;
#endif
#ifdef USE_IRIDESCENCE
	float dotNVi = saturate( dot( normal, geometryViewDir ) );
	if ( material.iridescenceThickness == 0.0 ) {
		material.iridescence = 0.0;
	} else {
		material.iridescence = saturate( material.iridescence );
	}
	if ( material.iridescence > 0.0 ) {
		material.iridescenceFresnel = evalIridescence( 1.0, material.iridescenceIOR, dotNVi, material.iridescenceThickness, material.specularColor );
		material.iridescenceF0 = Schlick_to_F0( material.iridescenceFresnel, 1.0, dotNVi );
	}
#endif
IncidentLight directLight;
#if ( NUM_POINT_LIGHTS > 0 ) && defined( RE_Direct )
	PointLight pointLight;
	#if defined( USE_SHADOWMAP ) && NUM_POINT_LIGHT_SHADOWS > 0
	PointLightShadow pointLightShadow;
	#endif
	#pragma unroll_loop_start
	for ( int i = 0; i < NUM_POINT_LIGHTS; i ++ ) {
		pointLight = pointLights[ i ];
		getPointLightInfo( pointLight, geometryPosition, directLight );
		#if defined( USE_SHADOWMAP ) && ( UNROLLED_LOOP_INDEX < NUM_POINT_LIGHT_SHADOWS )
		pointLightShadow = pointLightShadows[ i ];
		directLight.color *= ( directLight.visible && receiveShadow ) ? getPointShadow( pointShadowMap[ i ], pointLightShadow.shadowMapSize, pointLightShadow.shadowIntensity, pointLightShadow.shadowBias, pointLightShadow.shadowRadius, vPointShadowCoord[ i ], pointLightShadow.shadowCameraNear, pointLightShadow.shadowCameraFar ) : 1.0;
		#endif
		RE_Direct( directLight, geometryPosition, geometryNormal, geometryViewDir, geometryClearcoatNormal, material, reflectedLight );
	}
	#pragma unroll_loop_end
#endif
#if ( NUM_SPOT_LIGHTS > 0 ) && defined( RE_Direct )
	SpotLight spotLight;
	vec4 spotColor;
	vec3 spotLightCoord;
	bool inSpotLightMap;
	#if defined( USE_SHADOWMAP ) && NUM_SPOT_LIGHT_SHADOWS > 0
	SpotLightShadow spotLightShadow;
	#endif
	#pragma unroll_loop_start
	for ( int i = 0; i < NUM_SPOT_LIGHTS; i ++ ) {
		spotLight = spotLights[ i ];
		getSpotLightInfo( spotLight, geometryPosition, directLight );
		#if ( UNROLLED_LOOP_INDEX < NUM_SPOT_LIGHT_SHADOWS_WITH_MAPS )
		#define SPOT_LIGHT_MAP_INDEX UNROLLED_LOOP_INDEX
		#elif ( UNROLLED_LOOP_INDEX < NUM_SPOT_LIGHT_SHADOWS )
		#define SPOT_LIGHT_MAP_INDEX NUM_SPOT_LIGHT_MAPS
		#else
		#define SPOT_LIGHT_MAP_INDEX ( UNROLLED_LOOP_INDEX - NUM_SPOT_LIGHT_SHADOWS + NUM_SPOT_LIGHT_SHADOWS_WITH_MAPS )
		#endif
		#if ( SPOT_LIGHT_MAP_INDEX < NUM_SPOT_LIGHT_MAPS )
			spotLightCoord = vSpotLightCoord[ i ].xyz / vSpotLightCoord[ i ].w;
			inSpotLightMap = all( lessThan( abs( spotLightCoord * 2. - 1. ), vec3( 1.0 ) ) );
			spotColor = texture2D( spotLightMap[ SPOT_LIGHT_MAP_INDEX ], spotLightCoord.xy );
			directLight.color = inSpotLightMap ? directLight.color * spotColor.rgb : directLight.color;
		#endif
		#undef SPOT_LIGHT_MAP_INDEX
		#if defined( USE_SHADOWMAP ) && ( UNROLLED_LOOP_INDEX < NUM_SPOT_LIGHT_SHADOWS )
		spotLightShadow = spotLightShadows[ i ];
		directLight.color *= ( directLight.visible && receiveShadow ) ? getShadow( spotShadowMap[ i ], spotLightShadow.shadowMapSize, spotLightShadow.shadowIntensity, spotLightShadow.shadowBias, spotLightShadow.shadowRadius, vSpotLightCoord[ i ] ) : 1.0;
		#endif
		RE_Direct( directLight, geometryPosition, geometryNormal, geometryViewDir, geometryClearcoatNormal, material, reflectedLight );
	}
	#pragma unroll_loop_end
#endif
#if ( NUM_DIR_LIGHTS > 0 ) && defined( RE_Direct )
	DirectionalLight directionalLight;
	#if defined( USE_SHADOWMAP ) && NUM_DIR_LIGHT_SHADOWS > 0
	DirectionalLightShadow directionalLightShadow;
	#endif
	#pragma unroll_loop_start
	for ( int i = 0; i < NUM_DIR_LIGHTS; i ++ ) {
		directionalLight = directionalLights[ i ];
		getDirectionalLightInfo( directionalLight, directLight );
		#if defined( USE_SHADOWMAP ) && ( UNROLLED_LOOP_INDEX < NUM_DIR_LIGHT_SHADOWS )
		directionalLightShadow = directionalLightShadows[ i ];
		directLight.color *= ( directLight.visible && receiveShadow ) ? getShadow( directionalShadowMap[ i ], directionalLightShadow.shadowMapSize, directionalLightShadow.shadowIntensity, directionalLightShadow.shadowBias, directionalLightShadow.shadowRadius, vDirectionalShadowCoord[ i ] ) : 1.0;
		#endif
		RE_Direct( directLight, geometryPosition, geometryNormal, geometryViewDir, geometryClearcoatNormal, material, reflectedLight );
	}
	#pragma unroll_loop_end
#endif
#if ( NUM_RECT_AREA_LIGHTS > 0 ) && defined( RE_Direct_RectArea )
	RectAreaLight rectAreaLight;
	#pragma unroll_loop_start
	for ( int i = 0; i < NUM_RECT_AREA_LIGHTS; i ++ ) {
		rectAreaLight = rectAreaLights[ i ];
		RE_Direct_RectArea( rectAreaLight, geometryPosition, geometryNormal, geometryViewDir, geometryClearcoatNormal, material, reflectedLight );
	}
	#pragma unroll_loop_end
#endif
#if defined( RE_IndirectDiffuse )
	vec3 iblIrradiance = vec3( 0.0 );
	vec3 irradiance = getAmbientLightIrradiance( ambientLightColor );
	#if defined( USE_LIGHT_PROBES )
		irradiance += getLightProbeIrradiance( lightProbe, geometryNormal );
	#endif
	#if ( NUM_HEMI_LIGHTS > 0 )
		#pragma unroll_loop_start
		for ( int i = 0; i < NUM_HEMI_LIGHTS; i ++ ) {
			irradiance += getHemisphereLightIrradiance( hemisphereLights[ i ], geometryNormal );
		}
		#pragma unroll_loop_end
	#endif
#endif
#if defined( RE_IndirectSpecular )
	vec3 radiance = vec3( 0.0 );
	vec3 clearcoatRadiance = vec3( 0.0 );
#endif`,jM=`#if defined( RE_IndirectDiffuse )
	#ifdef USE_LIGHTMAP
		vec4 lightMapTexel = texture2D( lightMap, vLightMapUv );
		vec3 lightMapIrradiance = lightMapTexel.rgb * lightMapIntensity;
		irradiance += lightMapIrradiance;
	#endif
	#if defined( USE_ENVMAP ) && defined( STANDARD ) && defined( ENVMAP_TYPE_CUBE_UV )
		iblIrradiance += getIBLIrradiance( geometryNormal );
	#endif
#endif
#if defined( USE_ENVMAP ) && defined( RE_IndirectSpecular )
	#ifdef USE_ANISOTROPY
		radiance += getIBLAnisotropyRadiance( geometryViewDir, geometryNormal, material.roughness, material.anisotropyB, material.anisotropy );
	#else
		radiance += getIBLRadiance( geometryViewDir, geometryNormal, material.roughness );
	#endif
	#ifdef USE_CLEARCOAT
		clearcoatRadiance += getIBLRadiance( geometryViewDir, geometryClearcoatNormal, material.clearcoatRoughness );
	#endif
#endif`,qM=`#if defined( RE_IndirectDiffuse )
	RE_IndirectDiffuse( irradiance, geometryPosition, geometryNormal, geometryViewDir, geometryClearcoatNormal, material, reflectedLight );
#endif
#if defined( RE_IndirectSpecular )
	RE_IndirectSpecular( radiance, iblIrradiance, clearcoatRadiance, geometryPosition, geometryNormal, geometryViewDir, geometryClearcoatNormal, material, reflectedLight );
#endif`,YM=`#if defined( USE_LOGDEPTHBUF )
	gl_FragDepth = vIsPerspective == 0.0 ? gl_FragCoord.z : log2( vFragDepth ) * logDepthBufFC * 0.5;
#endif`,KM=`#if defined( USE_LOGDEPTHBUF )
	uniform float logDepthBufFC;
	varying float vFragDepth;
	varying float vIsPerspective;
#endif`,$M=`#ifdef USE_LOGDEPTHBUF
	varying float vFragDepth;
	varying float vIsPerspective;
#endif`,ZM=`#ifdef USE_LOGDEPTHBUF
	vFragDepth = 1.0 + gl_Position.w;
	vIsPerspective = float( isPerspectiveMatrix( projectionMatrix ) );
#endif`,JM=`#ifdef USE_MAP
	vec4 sampledDiffuseColor = texture2D( map, vMapUv );
	#ifdef DECODE_VIDEO_TEXTURE
		sampledDiffuseColor = vec4( mix( pow( sampledDiffuseColor.rgb * 0.9478672986 + vec3( 0.0521327014 ), vec3( 2.4 ) ), sampledDiffuseColor.rgb * 0.0773993808, vec3( lessThanEqual( sampledDiffuseColor.rgb, vec3( 0.04045 ) ) ) ), sampledDiffuseColor.w );
	
	#endif
	diffuseColor *= sampledDiffuseColor;
#endif`,QM=`#ifdef USE_MAP
	uniform sampler2D map;
#endif`,eE=`#if defined( USE_MAP ) || defined( USE_ALPHAMAP )
	#if defined( USE_POINTS_UV )
		vec2 uv = vUv;
	#else
		vec2 uv = ( uvTransform * vec3( gl_PointCoord.x, 1.0 - gl_PointCoord.y, 1 ) ).xy;
	#endif
#endif
#ifdef USE_MAP
	diffuseColor *= texture2D( map, uv );
#endif
#ifdef USE_ALPHAMAP
	diffuseColor.a *= texture2D( alphaMap, uv ).g;
#endif`,tE=`#if defined( USE_POINTS_UV )
	varying vec2 vUv;
#else
	#if defined( USE_MAP ) || defined( USE_ALPHAMAP )
		uniform mat3 uvTransform;
	#endif
#endif
#ifdef USE_MAP
	uniform sampler2D map;
#endif
#ifdef USE_ALPHAMAP
	uniform sampler2D alphaMap;
#endif`,nE=`float metalnessFactor = metalness;
#ifdef USE_METALNESSMAP
	vec4 texelMetalness = texture2D( metalnessMap, vMetalnessMapUv );
	metalnessFactor *= texelMetalness.b;
#endif`,iE=`#ifdef USE_METALNESSMAP
	uniform sampler2D metalnessMap;
#endif`,rE=`#ifdef USE_INSTANCING_MORPH
	float morphTargetInfluences[ MORPHTARGETS_COUNT ];
	float morphTargetBaseInfluence = texelFetch( morphTexture, ivec2( 0, gl_InstanceID ), 0 ).r;
	for ( int i = 0; i < MORPHTARGETS_COUNT; i ++ ) {
		morphTargetInfluences[i] =  texelFetch( morphTexture, ivec2( i + 1, gl_InstanceID ), 0 ).r;
	}
#endif`,sE=`#if defined( USE_MORPHCOLORS )
	vColor *= morphTargetBaseInfluence;
	for ( int i = 0; i < MORPHTARGETS_COUNT; i ++ ) {
		#if defined( USE_COLOR_ALPHA )
			if ( morphTargetInfluences[ i ] != 0.0 ) vColor += getMorph( gl_VertexID, i, 2 ) * morphTargetInfluences[ i ];
		#elif defined( USE_COLOR )
			if ( morphTargetInfluences[ i ] != 0.0 ) vColor += getMorph( gl_VertexID, i, 2 ).rgb * morphTargetInfluences[ i ];
		#endif
	}
#endif`,oE=`#ifdef USE_MORPHNORMALS
	objectNormal *= morphTargetBaseInfluence;
	for ( int i = 0; i < MORPHTARGETS_COUNT; i ++ ) {
		if ( morphTargetInfluences[ i ] != 0.0 ) objectNormal += getMorph( gl_VertexID, i, 1 ).xyz * morphTargetInfluences[ i ];
	}
#endif`,aE=`#ifdef USE_MORPHTARGETS
	#ifndef USE_INSTANCING_MORPH
		uniform float morphTargetBaseInfluence;
		uniform float morphTargetInfluences[ MORPHTARGETS_COUNT ];
	#endif
	uniform sampler2DArray morphTargetsTexture;
	uniform ivec2 morphTargetsTextureSize;
	vec4 getMorph( const in int vertexIndex, const in int morphTargetIndex, const in int offset ) {
		int texelIndex = vertexIndex * MORPHTARGETS_TEXTURE_STRIDE + offset;
		int y = texelIndex / morphTargetsTextureSize.x;
		int x = texelIndex - y * morphTargetsTextureSize.x;
		ivec3 morphUV = ivec3( x, y, morphTargetIndex );
		return texelFetch( morphTargetsTexture, morphUV, 0 );
	}
#endif`,lE=`#ifdef USE_MORPHTARGETS
	transformed *= morphTargetBaseInfluence;
	for ( int i = 0; i < MORPHTARGETS_COUNT; i ++ ) {
		if ( morphTargetInfluences[ i ] != 0.0 ) transformed += getMorph( gl_VertexID, i, 0 ).xyz * morphTargetInfluences[ i ];
	}
#endif`,uE=`float faceDirection = gl_FrontFacing ? 1.0 : - 1.0;
#ifdef FLAT_SHADED
	vec3 fdx = dFdx( vViewPosition );
	vec3 fdy = dFdy( vViewPosition );
	vec3 normal = normalize( cross( fdx, fdy ) );
#else
	vec3 normal = normalize( vNormal );
	#ifdef DOUBLE_SIDED
		normal *= faceDirection;
	#endif
#endif
#if defined( USE_NORMALMAP_TANGENTSPACE ) || defined( USE_CLEARCOAT_NORMALMAP ) || defined( USE_ANISOTROPY )
	#ifdef USE_TANGENT
		mat3 tbn = mat3( normalize( vTangent ), normalize( vBitangent ), normal );
	#else
		mat3 tbn = getTangentFrame( - vViewPosition, normal,
		#if defined( USE_NORMALMAP )
			vNormalMapUv
		#elif defined( USE_CLEARCOAT_NORMALMAP )
			vClearcoatNormalMapUv
		#else
			vUv
		#endif
		);
	#endif
	#if defined( DOUBLE_SIDED ) && ! defined( FLAT_SHADED )
		tbn[0] *= faceDirection;
		tbn[1] *= faceDirection;
	#endif
#endif
#ifdef USE_CLEARCOAT_NORMALMAP
	#ifdef USE_TANGENT
		mat3 tbn2 = mat3( normalize( vTangent ), normalize( vBitangent ), normal );
	#else
		mat3 tbn2 = getTangentFrame( - vViewPosition, normal, vClearcoatNormalMapUv );
	#endif
	#if defined( DOUBLE_SIDED ) && ! defined( FLAT_SHADED )
		tbn2[0] *= faceDirection;
		tbn2[1] *= faceDirection;
	#endif
#endif
vec3 nonPerturbedNormal = normal;`,cE=`#ifdef USE_NORMALMAP_OBJECTSPACE
	normal = texture2D( normalMap, vNormalMapUv ).xyz * 2.0 - 1.0;
	#ifdef FLIP_SIDED
		normal = - normal;
	#endif
	#ifdef DOUBLE_SIDED
		normal = normal * faceDirection;
	#endif
	normal = normalize( normalMatrix * normal );
#elif defined( USE_NORMALMAP_TANGENTSPACE )
	vec3 mapN = texture2D( normalMap, vNormalMapUv ).xyz * 2.0 - 1.0;
	mapN.xy *= normalScale;
	normal = normalize( tbn * mapN );
#elif defined( USE_BUMPMAP )
	normal = perturbNormalArb( - vViewPosition, normal, dHdxy_fwd(), faceDirection );
#endif`,fE=`#ifndef FLAT_SHADED
	varying vec3 vNormal;
	#ifdef USE_TANGENT
		varying vec3 vTangent;
		varying vec3 vBitangent;
	#endif
#endif`,hE=`#ifndef FLAT_SHADED
	varying vec3 vNormal;
	#ifdef USE_TANGENT
		varying vec3 vTangent;
		varying vec3 vBitangent;
	#endif
#endif`,dE=`#ifndef FLAT_SHADED
	vNormal = normalize( transformedNormal );
	#ifdef USE_TANGENT
		vTangent = normalize( transformedTangent );
		vBitangent = normalize( cross( vNormal, vTangent ) * tangent.w );
	#endif
#endif`,pE=`#ifdef USE_NORMALMAP
	uniform sampler2D normalMap;
	uniform vec2 normalScale;
#endif
#ifdef USE_NORMALMAP_OBJECTSPACE
	uniform mat3 normalMatrix;
#endif
#if ! defined ( USE_TANGENT ) && ( defined ( USE_NORMALMAP_TANGENTSPACE ) || defined ( USE_CLEARCOAT_NORMALMAP ) || defined( USE_ANISOTROPY ) )
	mat3 getTangentFrame( vec3 eye_pos, vec3 surf_norm, vec2 uv ) {
		vec3 q0 = dFdx( eye_pos.xyz );
		vec3 q1 = dFdy( eye_pos.xyz );
		vec2 st0 = dFdx( uv.st );
		vec2 st1 = dFdy( uv.st );
		vec3 N = surf_norm;
		vec3 q1perp = cross( q1, N );
		vec3 q0perp = cross( N, q0 );
		vec3 T = q1perp * st0.x + q0perp * st1.x;
		vec3 B = q1perp * st0.y + q0perp * st1.y;
		float det = max( dot( T, T ), dot( B, B ) );
		float scale = ( det == 0.0 ) ? 0.0 : inversesqrt( det );
		return mat3( T * scale, B * scale, N );
	}
#endif`,mE=`#ifdef USE_CLEARCOAT
	vec3 clearcoatNormal = nonPerturbedNormal;
#endif`,gE=`#ifdef USE_CLEARCOAT_NORMALMAP
	vec3 clearcoatMapN = texture2D( clearcoatNormalMap, vClearcoatNormalMapUv ).xyz * 2.0 - 1.0;
	clearcoatMapN.xy *= clearcoatNormalScale;
	clearcoatNormal = normalize( tbn2 * clearcoatMapN );
#endif`,_E=`#ifdef USE_CLEARCOATMAP
	uniform sampler2D clearcoatMap;
#endif
#ifdef USE_CLEARCOAT_NORMALMAP
	uniform sampler2D clearcoatNormalMap;
	uniform vec2 clearcoatNormalScale;
#endif
#ifdef USE_CLEARCOAT_ROUGHNESSMAP
	uniform sampler2D clearcoatRoughnessMap;
#endif`,vE=`#ifdef USE_IRIDESCENCEMAP
	uniform sampler2D iridescenceMap;
#endif
#ifdef USE_IRIDESCENCE_THICKNESSMAP
	uniform sampler2D iridescenceThicknessMap;
#endif`,yE=`#ifdef OPAQUE
diffuseColor.a = 1.0;
#endif
#ifdef USE_TRANSMISSION
diffuseColor.a *= material.transmissionAlpha;
#endif
gl_FragColor = vec4( outgoingLight, diffuseColor.a );`,xE=`vec3 packNormalToRGB( const in vec3 normal ) {
	return normalize( normal ) * 0.5 + 0.5;
}
vec3 unpackRGBToNormal( const in vec3 rgb ) {
	return 2.0 * rgb.xyz - 1.0;
}
const float PackUpscale = 256. / 255.;const float UnpackDownscale = 255. / 256.;const float ShiftRight8 = 1. / 256.;
const float Inv255 = 1. / 255.;
const vec4 PackFactors = vec4( 1.0, 256.0, 256.0 * 256.0, 256.0 * 256.0 * 256.0 );
const vec2 UnpackFactors2 = vec2( UnpackDownscale, 1.0 / PackFactors.g );
const vec3 UnpackFactors3 = vec3( UnpackDownscale / PackFactors.rg, 1.0 / PackFactors.b );
const vec4 UnpackFactors4 = vec4( UnpackDownscale / PackFactors.rgb, 1.0 / PackFactors.a );
vec4 packDepthToRGBA( const in float v ) {
	if( v <= 0.0 )
		return vec4( 0., 0., 0., 0. );
	if( v >= 1.0 )
		return vec4( 1., 1., 1., 1. );
	float vuf;
	float af = modf( v * PackFactors.a, vuf );
	float bf = modf( vuf * ShiftRight8, vuf );
	float gf = modf( vuf * ShiftRight8, vuf );
	return vec4( vuf * Inv255, gf * PackUpscale, bf * PackUpscale, af );
}
vec3 packDepthToRGB( const in float v ) {
	if( v <= 0.0 )
		return vec3( 0., 0., 0. );
	if( v >= 1.0 )
		return vec3( 1., 1., 1. );
	float vuf;
	float bf = modf( v * PackFactors.b, vuf );
	float gf = modf( vuf * ShiftRight8, vuf );
	return vec3( vuf * Inv255, gf * PackUpscale, bf );
}
vec2 packDepthToRG( const in float v ) {
	if( v <= 0.0 )
		return vec2( 0., 0. );
	if( v >= 1.0 )
		return vec2( 1., 1. );
	float vuf;
	float gf = modf( v * 256., vuf );
	return vec2( vuf * Inv255, gf );
}
float unpackRGBAToDepth( const in vec4 v ) {
	return dot( v, UnpackFactors4 );
}
float unpackRGBToDepth( const in vec3 v ) {
	return dot( v, UnpackFactors3 );
}
float unpackRGToDepth( const in vec2 v ) {
	return v.r * UnpackFactors2.r + v.g * UnpackFactors2.g;
}
vec4 pack2HalfToRGBA( const in vec2 v ) {
	vec4 r = vec4( v.x, fract( v.x * 255.0 ), v.y, fract( v.y * 255.0 ) );
	return vec4( r.x - r.y / 255.0, r.y, r.z - r.w / 255.0, r.w );
}
vec2 unpackRGBATo2Half( const in vec4 v ) {
	return vec2( v.x + ( v.y / 255.0 ), v.z + ( v.w / 255.0 ) );
}
float viewZToOrthographicDepth( const in float viewZ, const in float near, const in float far ) {
	return ( viewZ + near ) / ( near - far );
}
float orthographicDepthToViewZ( const in float depth, const in float near, const in float far ) {
	return depth * ( near - far ) - near;
}
float viewZToPerspectiveDepth( const in float viewZ, const in float near, const in float far ) {
	return ( ( near + viewZ ) * far ) / ( ( far - near ) * viewZ );
}
float perspectiveDepthToViewZ( const in float depth, const in float near, const in float far ) {
	return ( near * far ) / ( ( far - near ) * depth - far );
}`,SE=`#ifdef PREMULTIPLIED_ALPHA
	gl_FragColor.rgb *= gl_FragColor.a;
#endif`,ME=`vec4 mvPosition = vec4( transformed, 1.0 );
#ifdef USE_BATCHING
	mvPosition = batchingMatrix * mvPosition;
#endif
#ifdef USE_INSTANCING
	mvPosition = instanceMatrix * mvPosition;
#endif
mvPosition = modelViewMatrix * mvPosition;
gl_Position = projectionMatrix * mvPosition;`,EE=`#ifdef DITHERING
	gl_FragColor.rgb = dithering( gl_FragColor.rgb );
#endif`,wE=`#ifdef DITHERING
	vec3 dithering( vec3 color ) {
		float grid_position = rand( gl_FragCoord.xy );
		vec3 dither_shift_RGB = vec3( 0.25 / 255.0, -0.25 / 255.0, 0.25 / 255.0 );
		dither_shift_RGB = mix( 2.0 * dither_shift_RGB, -2.0 * dither_shift_RGB, grid_position );
		return color + dither_shift_RGB;
	}
#endif`,TE=`float roughnessFactor = roughness;
#ifdef USE_ROUGHNESSMAP
	vec4 texelRoughness = texture2D( roughnessMap, vRoughnessMapUv );
	roughnessFactor *= texelRoughness.g;
#endif`,AE=`#ifdef USE_ROUGHNESSMAP
	uniform sampler2D roughnessMap;
#endif`,CE=`#if NUM_SPOT_LIGHT_COORDS > 0
	varying vec4 vSpotLightCoord[ NUM_SPOT_LIGHT_COORDS ];
#endif
#if NUM_SPOT_LIGHT_MAPS > 0
	uniform sampler2D spotLightMap[ NUM_SPOT_LIGHT_MAPS ];
#endif
#ifdef USE_SHADOWMAP
	#if NUM_DIR_LIGHT_SHADOWS > 0
		uniform sampler2D directionalShadowMap[ NUM_DIR_LIGHT_SHADOWS ];
		varying vec4 vDirectionalShadowCoord[ NUM_DIR_LIGHT_SHADOWS ];
		struct DirectionalLightShadow {
			float shadowIntensity;
			float shadowBias;
			float shadowNormalBias;
			float shadowRadius;
			vec2 shadowMapSize;
		};
		uniform DirectionalLightShadow directionalLightShadows[ NUM_DIR_LIGHT_SHADOWS ];
	#endif
	#if NUM_SPOT_LIGHT_SHADOWS > 0
		uniform sampler2D spotShadowMap[ NUM_SPOT_LIGHT_SHADOWS ];
		struct SpotLightShadow {
			float shadowIntensity;
			float shadowBias;
			float shadowNormalBias;
			float shadowRadius;
			vec2 shadowMapSize;
		};
		uniform SpotLightShadow spotLightShadows[ NUM_SPOT_LIGHT_SHADOWS ];
	#endif
	#if NUM_POINT_LIGHT_SHADOWS > 0
		uniform sampler2D pointShadowMap[ NUM_POINT_LIGHT_SHADOWS ];
		varying vec4 vPointShadowCoord[ NUM_POINT_LIGHT_SHADOWS ];
		struct PointLightShadow {
			float shadowIntensity;
			float shadowBias;
			float shadowNormalBias;
			float shadowRadius;
			vec2 shadowMapSize;
			float shadowCameraNear;
			float shadowCameraFar;
		};
		uniform PointLightShadow pointLightShadows[ NUM_POINT_LIGHT_SHADOWS ];
	#endif
	float texture2DCompare( sampler2D depths, vec2 uv, float compare ) {
		return step( compare, unpackRGBAToDepth( texture2D( depths, uv ) ) );
	}
	vec2 texture2DDistribution( sampler2D shadow, vec2 uv ) {
		return unpackRGBATo2Half( texture2D( shadow, uv ) );
	}
	float VSMShadow (sampler2D shadow, vec2 uv, float compare ){
		float occlusion = 1.0;
		vec2 distribution = texture2DDistribution( shadow, uv );
		float hard_shadow = step( compare , distribution.x );
		if (hard_shadow != 1.0 ) {
			float distance = compare - distribution.x ;
			float variance = max( 0.00000, distribution.y * distribution.y );
			float softness_probability = variance / (variance + distance * distance );			softness_probability = clamp( ( softness_probability - 0.3 ) / ( 0.95 - 0.3 ), 0.0, 1.0 );			occlusion = clamp( max( hard_shadow, softness_probability ), 0.0, 1.0 );
		}
		return occlusion;
	}
	float getShadow( sampler2D shadowMap, vec2 shadowMapSize, float shadowIntensity, float shadowBias, float shadowRadius, vec4 shadowCoord ) {
		float shadow = 1.0;
		shadowCoord.xyz /= shadowCoord.w;
		shadowCoord.z += shadowBias;
		bool inFrustum = shadowCoord.x >= 0.0 && shadowCoord.x <= 1.0 && shadowCoord.y >= 0.0 && shadowCoord.y <= 1.0;
		bool frustumTest = inFrustum && shadowCoord.z <= 1.0;
		if ( frustumTest ) {
		#if defined( SHADOWMAP_TYPE_PCF )
			vec2 texelSize = vec2( 1.0 ) / shadowMapSize;
			float dx0 = - texelSize.x * shadowRadius;
			float dy0 = - texelSize.y * shadowRadius;
			float dx1 = + texelSize.x * shadowRadius;
			float dy1 = + texelSize.y * shadowRadius;
			float dx2 = dx0 / 2.0;
			float dy2 = dy0 / 2.0;
			float dx3 = dx1 / 2.0;
			float dy3 = dy1 / 2.0;
			shadow = (
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( dx0, dy0 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( 0.0, dy0 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( dx1, dy0 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( dx2, dy2 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( 0.0, dy2 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( dx3, dy2 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( dx0, 0.0 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( dx2, 0.0 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy, shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( dx3, 0.0 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( dx1, 0.0 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( dx2, dy3 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( 0.0, dy3 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( dx3, dy3 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( dx0, dy1 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( 0.0, dy1 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, shadowCoord.xy + vec2( dx1, dy1 ), shadowCoord.z )
			) * ( 1.0 / 17.0 );
		#elif defined( SHADOWMAP_TYPE_PCF_SOFT )
			vec2 texelSize = vec2( 1.0 ) / shadowMapSize;
			float dx = texelSize.x;
			float dy = texelSize.y;
			vec2 uv = shadowCoord.xy;
			vec2 f = fract( uv * shadowMapSize + 0.5 );
			uv -= f * texelSize;
			shadow = (
				texture2DCompare( shadowMap, uv, shadowCoord.z ) +
				texture2DCompare( shadowMap, uv + vec2( dx, 0.0 ), shadowCoord.z ) +
				texture2DCompare( shadowMap, uv + vec2( 0.0, dy ), shadowCoord.z ) +
				texture2DCompare( shadowMap, uv + texelSize, shadowCoord.z ) +
				mix( texture2DCompare( shadowMap, uv + vec2( -dx, 0.0 ), shadowCoord.z ),
					 texture2DCompare( shadowMap, uv + vec2( 2.0 * dx, 0.0 ), shadowCoord.z ),
					 f.x ) +
				mix( texture2DCompare( shadowMap, uv + vec2( -dx, dy ), shadowCoord.z ),
					 texture2DCompare( shadowMap, uv + vec2( 2.0 * dx, dy ), shadowCoord.z ),
					 f.x ) +
				mix( texture2DCompare( shadowMap, uv + vec2( 0.0, -dy ), shadowCoord.z ),
					 texture2DCompare( shadowMap, uv + vec2( 0.0, 2.0 * dy ), shadowCoord.z ),
					 f.y ) +
				mix( texture2DCompare( shadowMap, uv + vec2( dx, -dy ), shadowCoord.z ),
					 texture2DCompare( shadowMap, uv + vec2( dx, 2.0 * dy ), shadowCoord.z ),
					 f.y ) +
				mix( mix( texture2DCompare( shadowMap, uv + vec2( -dx, -dy ), shadowCoord.z ),
						  texture2DCompare( shadowMap, uv + vec2( 2.0 * dx, -dy ), shadowCoord.z ),
						  f.x ),
					 mix( texture2DCompare( shadowMap, uv + vec2( -dx, 2.0 * dy ), shadowCoord.z ),
						  texture2DCompare( shadowMap, uv + vec2( 2.0 * dx, 2.0 * dy ), shadowCoord.z ),
						  f.x ),
					 f.y )
			) * ( 1.0 / 9.0 );
		#elif defined( SHADOWMAP_TYPE_VSM )
			shadow = VSMShadow( shadowMap, shadowCoord.xy, shadowCoord.z );
		#else
			shadow = texture2DCompare( shadowMap, shadowCoord.xy, shadowCoord.z );
		#endif
		}
		return mix( 1.0, shadow, shadowIntensity );
	}
	vec2 cubeToUV( vec3 v, float texelSizeY ) {
		vec3 absV = abs( v );
		float scaleToCube = 1.0 / max( absV.x, max( absV.y, absV.z ) );
		absV *= scaleToCube;
		v *= scaleToCube * ( 1.0 - 2.0 * texelSizeY );
		vec2 planar = v.xy;
		float almostATexel = 1.5 * texelSizeY;
		float almostOne = 1.0 - almostATexel;
		if ( absV.z >= almostOne ) {
			if ( v.z > 0.0 )
				planar.x = 4.0 - v.x;
		} else if ( absV.x >= almostOne ) {
			float signX = sign( v.x );
			planar.x = v.z * signX + 2.0 * signX;
		} else if ( absV.y >= almostOne ) {
			float signY = sign( v.y );
			planar.x = v.x + 2.0 * signY + 2.0;
			planar.y = v.z * signY - 2.0;
		}
		return vec2( 0.125, 0.25 ) * planar + vec2( 0.375, 0.75 );
	}
	float getPointShadow( sampler2D shadowMap, vec2 shadowMapSize, float shadowIntensity, float shadowBias, float shadowRadius, vec4 shadowCoord, float shadowCameraNear, float shadowCameraFar ) {
		float shadow = 1.0;
		vec3 lightToPosition = shadowCoord.xyz;
		
		float lightToPositionLength = length( lightToPosition );
		if ( lightToPositionLength - shadowCameraFar <= 0.0 && lightToPositionLength - shadowCameraNear >= 0.0 ) {
			float dp = ( lightToPositionLength - shadowCameraNear ) / ( shadowCameraFar - shadowCameraNear );			dp += shadowBias;
			vec3 bd3D = normalize( lightToPosition );
			vec2 texelSize = vec2( 1.0 ) / ( shadowMapSize * vec2( 4.0, 2.0 ) );
			#if defined( SHADOWMAP_TYPE_PCF ) || defined( SHADOWMAP_TYPE_PCF_SOFT ) || defined( SHADOWMAP_TYPE_VSM )
				vec2 offset = vec2( - 1, 1 ) * shadowRadius * texelSize.y;
				shadow = (
					texture2DCompare( shadowMap, cubeToUV( bd3D + offset.xyy, texelSize.y ), dp ) +
					texture2DCompare( shadowMap, cubeToUV( bd3D + offset.yyy, texelSize.y ), dp ) +
					texture2DCompare( shadowMap, cubeToUV( bd3D + offset.xyx, texelSize.y ), dp ) +
					texture2DCompare( shadowMap, cubeToUV( bd3D + offset.yyx, texelSize.y ), dp ) +
					texture2DCompare( shadowMap, cubeToUV( bd3D, texelSize.y ), dp ) +
					texture2DCompare( shadowMap, cubeToUV( bd3D + offset.xxy, texelSize.y ), dp ) +
					texture2DCompare( shadowMap, cubeToUV( bd3D + offset.yxy, texelSize.y ), dp ) +
					texture2DCompare( shadowMap, cubeToUV( bd3D + offset.xxx, texelSize.y ), dp ) +
					texture2DCompare( shadowMap, cubeToUV( bd3D + offset.yxx, texelSize.y ), dp )
				) * ( 1.0 / 9.0 );
			#else
				shadow = texture2DCompare( shadowMap, cubeToUV( bd3D, texelSize.y ), dp );
			#endif
		}
		return mix( 1.0, shadow, shadowIntensity );
	}
#endif`,RE=`#if NUM_SPOT_LIGHT_COORDS > 0
	uniform mat4 spotLightMatrix[ NUM_SPOT_LIGHT_COORDS ];
	varying vec4 vSpotLightCoord[ NUM_SPOT_LIGHT_COORDS ];
#endif
#ifdef USE_SHADOWMAP
	#if NUM_DIR_LIGHT_SHADOWS > 0
		uniform mat4 directionalShadowMatrix[ NUM_DIR_LIGHT_SHADOWS ];
		varying vec4 vDirectionalShadowCoord[ NUM_DIR_LIGHT_SHADOWS ];
		struct DirectionalLightShadow {
			float shadowIntensity;
			float shadowBias;
			float shadowNormalBias;
			float shadowRadius;
			vec2 shadowMapSize;
		};
		uniform DirectionalLightShadow directionalLightShadows[ NUM_DIR_LIGHT_SHADOWS ];
	#endif
	#if NUM_SPOT_LIGHT_SHADOWS > 0
		struct SpotLightShadow {
			float shadowIntensity;
			float shadowBias;
			float shadowNormalBias;
			float shadowRadius;
			vec2 shadowMapSize;
		};
		uniform SpotLightShadow spotLightShadows[ NUM_SPOT_LIGHT_SHADOWS ];
	#endif
	#if NUM_POINT_LIGHT_SHADOWS > 0
		uniform mat4 pointShadowMatrix[ NUM_POINT_LIGHT_SHADOWS ];
		varying vec4 vPointShadowCoord[ NUM_POINT_LIGHT_SHADOWS ];
		struct PointLightShadow {
			float shadowIntensity;
			float shadowBias;
			float shadowNormalBias;
			float shadowRadius;
			vec2 shadowMapSize;
			float shadowCameraNear;
			float shadowCameraFar;
		};
		uniform PointLightShadow pointLightShadows[ NUM_POINT_LIGHT_SHADOWS ];
	#endif
#endif`,PE=`#if ( defined( USE_SHADOWMAP ) && ( NUM_DIR_LIGHT_SHADOWS > 0 || NUM_POINT_LIGHT_SHADOWS > 0 ) ) || ( NUM_SPOT_LIGHT_COORDS > 0 )
	vec3 shadowWorldNormal = inverseTransformDirection( transformedNormal, viewMatrix );
	vec4 shadowWorldPosition;
#endif
#if defined( USE_SHADOWMAP )
	#if NUM_DIR_LIGHT_SHADOWS > 0
		#pragma unroll_loop_start
		for ( int i = 0; i < NUM_DIR_LIGHT_SHADOWS; i ++ ) {
			shadowWorldPosition = worldPosition + vec4( shadowWorldNormal * directionalLightShadows[ i ].shadowNormalBias, 0 );
			vDirectionalShadowCoord[ i ] = directionalShadowMatrix[ i ] * shadowWorldPosition;
		}
		#pragma unroll_loop_end
	#endif
	#if NUM_POINT_LIGHT_SHADOWS > 0
		#pragma unroll_loop_start
		for ( int i = 0; i < NUM_POINT_LIGHT_SHADOWS; i ++ ) {
			shadowWorldPosition = worldPosition + vec4( shadowWorldNormal * pointLightShadows[ i ].shadowNormalBias, 0 );
			vPointShadowCoord[ i ] = pointShadowMatrix[ i ] * shadowWorldPosition;
		}
		#pragma unroll_loop_end
	#endif
#endif
#if NUM_SPOT_LIGHT_COORDS > 0
	#pragma unroll_loop_start
	for ( int i = 0; i < NUM_SPOT_LIGHT_COORDS; i ++ ) {
		shadowWorldPosition = worldPosition;
		#if ( defined( USE_SHADOWMAP ) && UNROLLED_LOOP_INDEX < NUM_SPOT_LIGHT_SHADOWS )
			shadowWorldPosition.xyz += shadowWorldNormal * spotLightShadows[ i ].shadowNormalBias;
		#endif
		vSpotLightCoord[ i ] = spotLightMatrix[ i ] * shadowWorldPosition;
	}
	#pragma unroll_loop_end
#endif`,bE=`float getShadowMask() {
	float shadow = 1.0;
	#ifdef USE_SHADOWMAP
	#if NUM_DIR_LIGHT_SHADOWS > 0
	DirectionalLightShadow directionalLight;
	#pragma unroll_loop_start
	for ( int i = 0; i < NUM_DIR_LIGHT_SHADOWS; i ++ ) {
		directionalLight = directionalLightShadows[ i ];
		shadow *= receiveShadow ? getShadow( directionalShadowMap[ i ], directionalLight.shadowMapSize, directionalLight.shadowIntensity, directionalLight.shadowBias, directionalLight.shadowRadius, vDirectionalShadowCoord[ i ] ) : 1.0;
	}
	#pragma unroll_loop_end
	#endif
	#if NUM_SPOT_LIGHT_SHADOWS > 0
	SpotLightShadow spotLight;
	#pragma unroll_loop_start
	for ( int i = 0; i < NUM_SPOT_LIGHT_SHADOWS; i ++ ) {
		spotLight = spotLightShadows[ i ];
		shadow *= receiveShadow ? getShadow( spotShadowMap[ i ], spotLight.shadowMapSize, spotLight.shadowIntensity, spotLight.shadowBias, spotLight.shadowRadius, vSpotLightCoord[ i ] ) : 1.0;
	}
	#pragma unroll_loop_end
	#endif
	#if NUM_POINT_LIGHT_SHADOWS > 0
	PointLightShadow pointLight;
	#pragma unroll_loop_start
	for ( int i = 0; i < NUM_POINT_LIGHT_SHADOWS; i ++ ) {
		pointLight = pointLightShadows[ i ];
		shadow *= receiveShadow ? getPointShadow( pointShadowMap[ i ], pointLight.shadowMapSize, pointLight.shadowIntensity, pointLight.shadowBias, pointLight.shadowRadius, vPointShadowCoord[ i ], pointLight.shadowCameraNear, pointLight.shadowCameraFar ) : 1.0;
	}
	#pragma unroll_loop_end
	#endif
	#endif
	return shadow;
}`,LE=`#ifdef USE_SKINNING
	mat4 boneMatX = getBoneMatrix( skinIndex.x );
	mat4 boneMatY = getBoneMatrix( skinIndex.y );
	mat4 boneMatZ = getBoneMatrix( skinIndex.z );
	mat4 boneMatW = getBoneMatrix( skinIndex.w );
#endif`,DE=`#ifdef USE_SKINNING
	uniform mat4 bindMatrix;
	uniform mat4 bindMatrixInverse;
	uniform highp sampler2D boneTexture;
	mat4 getBoneMatrix( const in float i ) {
		int size = textureSize( boneTexture, 0 ).x;
		int j = int( i ) * 4;
		int x = j % size;
		int y = j / size;
		vec4 v1 = texelFetch( boneTexture, ivec2( x, y ), 0 );
		vec4 v2 = texelFetch( boneTexture, ivec2( x + 1, y ), 0 );
		vec4 v3 = texelFetch( boneTexture, ivec2( x + 2, y ), 0 );
		vec4 v4 = texelFetch( boneTexture, ivec2( x + 3, y ), 0 );
		return mat4( v1, v2, v3, v4 );
	}
#endif`,NE=`#ifdef USE_SKINNING
	vec4 skinVertex = bindMatrix * vec4( transformed, 1.0 );
	vec4 skinned = vec4( 0.0 );
	skinned += boneMatX * skinVertex * skinWeight.x;
	skinned += boneMatY * skinVertex * skinWeight.y;
	skinned += boneMatZ * skinVertex * skinWeight.z;
	skinned += boneMatW * skinVertex * skinWeight.w;
	transformed = ( bindMatrixInverse * skinned ).xyz;
#endif`,IE=`#ifdef USE_SKINNING
	mat4 skinMatrix = mat4( 0.0 );
	skinMatrix += skinWeight.x * boneMatX;
	skinMatrix += skinWeight.y * boneMatY;
	skinMatrix += skinWeight.z * boneMatZ;
	skinMatrix += skinWeight.w * boneMatW;
	skinMatrix = bindMatrixInverse * skinMatrix * bindMatrix;
	objectNormal = vec4( skinMatrix * vec4( objectNormal, 0.0 ) ).xyz;
	#ifdef USE_TANGENT
		objectTangent = vec4( skinMatrix * vec4( objectTangent, 0.0 ) ).xyz;
	#endif
#endif`,UE=`float specularStrength;
#ifdef USE_SPECULARMAP
	vec4 texelSpecular = texture2D( specularMap, vSpecularMapUv );
	specularStrength = texelSpecular.r;
#else
	specularStrength = 1.0;
#endif`,FE=`#ifdef USE_SPECULARMAP
	uniform sampler2D specularMap;
#endif`,OE=`#if defined( TONE_MAPPING )
	gl_FragColor.rgb = toneMapping( gl_FragColor.rgb );
#endif`,BE=`#ifndef saturate
#define saturate( a ) clamp( a, 0.0, 1.0 )
#endif
uniform float toneMappingExposure;
vec3 LinearToneMapping( vec3 color ) {
	return saturate( toneMappingExposure * color );
}
vec3 ReinhardToneMapping( vec3 color ) {
	color *= toneMappingExposure;
	return saturate( color / ( vec3( 1.0 ) + color ) );
}
vec3 CineonToneMapping( vec3 color ) {
	color *= toneMappingExposure;
	color = max( vec3( 0.0 ), color - 0.004 );
	return pow( ( color * ( 6.2 * color + 0.5 ) ) / ( color * ( 6.2 * color + 1.7 ) + 0.06 ), vec3( 2.2 ) );
}
vec3 RRTAndODTFit( vec3 v ) {
	vec3 a = v * ( v + 0.0245786 ) - 0.000090537;
	vec3 b = v * ( 0.983729 * v + 0.4329510 ) + 0.238081;
	return a / b;
}
vec3 ACESFilmicToneMapping( vec3 color ) {
	const mat3 ACESInputMat = mat3(
		vec3( 0.59719, 0.07600, 0.02840 ),		vec3( 0.35458, 0.90834, 0.13383 ),
		vec3( 0.04823, 0.01566, 0.83777 )
	);
	const mat3 ACESOutputMat = mat3(
		vec3(  1.60475, -0.10208, -0.00327 ),		vec3( -0.53108,  1.10813, -0.07276 ),
		vec3( -0.07367, -0.00605,  1.07602 )
	);
	color *= toneMappingExposure / 0.6;
	color = ACESInputMat * color;
	color = RRTAndODTFit( color );
	color = ACESOutputMat * color;
	return saturate( color );
}
const mat3 LINEAR_REC2020_TO_LINEAR_SRGB = mat3(
	vec3( 1.6605, - 0.1246, - 0.0182 ),
	vec3( - 0.5876, 1.1329, - 0.1006 ),
	vec3( - 0.0728, - 0.0083, 1.1187 )
);
const mat3 LINEAR_SRGB_TO_LINEAR_REC2020 = mat3(
	vec3( 0.6274, 0.0691, 0.0164 ),
	vec3( 0.3293, 0.9195, 0.0880 ),
	vec3( 0.0433, 0.0113, 0.8956 )
);
vec3 agxDefaultContrastApprox( vec3 x ) {
	vec3 x2 = x * x;
	vec3 x4 = x2 * x2;
	return + 15.5 * x4 * x2
		- 40.14 * x4 * x
		+ 31.96 * x4
		- 6.868 * x2 * x
		+ 0.4298 * x2
		+ 0.1191 * x
		- 0.00232;
}
vec3 AgXToneMapping( vec3 color ) {
	const mat3 AgXInsetMatrix = mat3(
		vec3( 0.856627153315983, 0.137318972929847, 0.11189821299995 ),
		vec3( 0.0951212405381588, 0.761241990602591, 0.0767994186031903 ),
		vec3( 0.0482516061458583, 0.101439036467562, 0.811302368396859 )
	);
	const mat3 AgXOutsetMatrix = mat3(
		vec3( 1.1271005818144368, - 0.1413297634984383, - 0.14132976349843826 ),
		vec3( - 0.11060664309660323, 1.157823702216272, - 0.11060664309660294 ),
		vec3( - 0.016493938717834573, - 0.016493938717834257, 1.2519364065950405 )
	);
	const float AgxMinEv = - 12.47393;	const float AgxMaxEv = 4.026069;
	color *= toneMappingExposure;
	color = LINEAR_SRGB_TO_LINEAR_REC2020 * color;
	color = AgXInsetMatrix * color;
	color = max( color, 1e-10 );	color = log2( color );
	color = ( color - AgxMinEv ) / ( AgxMaxEv - AgxMinEv );
	color = clamp( color, 0.0, 1.0 );
	color = agxDefaultContrastApprox( color );
	color = AgXOutsetMatrix * color;
	color = pow( max( vec3( 0.0 ), color ), vec3( 2.2 ) );
	color = LINEAR_REC2020_TO_LINEAR_SRGB * color;
	color = clamp( color, 0.0, 1.0 );
	return color;
}
vec3 NeutralToneMapping( vec3 color ) {
	const float StartCompression = 0.8 - 0.04;
	const float Desaturation = 0.15;
	color *= toneMappingExposure;
	float x = min( color.r, min( color.g, color.b ) );
	float offset = x < 0.08 ? x - 6.25 * x * x : 0.04;
	color -= offset;
	float peak = max( color.r, max( color.g, color.b ) );
	if ( peak < StartCompression ) return color;
	float d = 1. - StartCompression;
	float newPeak = 1. - d * d / ( peak + d - StartCompression );
	color *= newPeak / peak;
	float g = 1. - 1. / ( Desaturation * ( peak - newPeak ) + 1. );
	return mix( color, vec3( newPeak ), g );
}
vec3 CustomToneMapping( vec3 color ) { return color; }`,zE=`#ifdef USE_TRANSMISSION
	material.transmission = transmission;
	material.transmissionAlpha = 1.0;
	material.thickness = thickness;
	material.attenuationDistance = attenuationDistance;
	material.attenuationColor = attenuationColor;
	#ifdef USE_TRANSMISSIONMAP
		material.transmission *= texture2D( transmissionMap, vTransmissionMapUv ).r;
	#endif
	#ifdef USE_THICKNESSMAP
		material.thickness *= texture2D( thicknessMap, vThicknessMapUv ).g;
	#endif
	vec3 pos = vWorldPosition;
	vec3 v = normalize( cameraPosition - pos );
	vec3 n = inverseTransformDirection( normal, viewMatrix );
	vec4 transmitted = getIBLVolumeRefraction(
		n, v, material.roughness, material.diffuseColor, material.specularColor, material.specularF90,
		pos, modelMatrix, viewMatrix, projectionMatrix, material.dispersion, material.ior, material.thickness,
		material.attenuationColor, material.attenuationDistance );
	material.transmissionAlpha = mix( material.transmissionAlpha, transmitted.a, material.transmission );
	totalDiffuse = mix( totalDiffuse, transmitted.rgb, material.transmission );
#endif`,kE=`#ifdef USE_TRANSMISSION
	uniform float transmission;
	uniform float thickness;
	uniform float attenuationDistance;
	uniform vec3 attenuationColor;
	#ifdef USE_TRANSMISSIONMAP
		uniform sampler2D transmissionMap;
	#endif
	#ifdef USE_THICKNESSMAP
		uniform sampler2D thicknessMap;
	#endif
	uniform vec2 transmissionSamplerSize;
	uniform sampler2D transmissionSamplerMap;
	uniform mat4 modelMatrix;
	uniform mat4 projectionMatrix;
	varying vec3 vWorldPosition;
	float w0( float a ) {
		return ( 1.0 / 6.0 ) * ( a * ( a * ( - a + 3.0 ) - 3.0 ) + 1.0 );
	}
	float w1( float a ) {
		return ( 1.0 / 6.0 ) * ( a *  a * ( 3.0 * a - 6.0 ) + 4.0 );
	}
	float w2( float a ){
		return ( 1.0 / 6.0 ) * ( a * ( a * ( - 3.0 * a + 3.0 ) + 3.0 ) + 1.0 );
	}
	float w3( float a ) {
		return ( 1.0 / 6.0 ) * ( a * a * a );
	}
	float g0( float a ) {
		return w0( a ) + w1( a );
	}
	float g1( float a ) {
		return w2( a ) + w3( a );
	}
	float h0( float a ) {
		return - 1.0 + w1( a ) / ( w0( a ) + w1( a ) );
	}
	float h1( float a ) {
		return 1.0 + w3( a ) / ( w2( a ) + w3( a ) );
	}
	vec4 bicubic( sampler2D tex, vec2 uv, vec4 texelSize, float lod ) {
		uv = uv * texelSize.zw + 0.5;
		vec2 iuv = floor( uv );
		vec2 fuv = fract( uv );
		float g0x = g0( fuv.x );
		float g1x = g1( fuv.x );
		float h0x = h0( fuv.x );
		float h1x = h1( fuv.x );
		float h0y = h0( fuv.y );
		float h1y = h1( fuv.y );
		vec2 p0 = ( vec2( iuv.x + h0x, iuv.y + h0y ) - 0.5 ) * texelSize.xy;
		vec2 p1 = ( vec2( iuv.x + h1x, iuv.y + h0y ) - 0.5 ) * texelSize.xy;
		vec2 p2 = ( vec2( iuv.x + h0x, iuv.y + h1y ) - 0.5 ) * texelSize.xy;
		vec2 p3 = ( vec2( iuv.x + h1x, iuv.y + h1y ) - 0.5 ) * texelSize.xy;
		return g0( fuv.y ) * ( g0x * textureLod( tex, p0, lod ) + g1x * textureLod( tex, p1, lod ) ) +
			g1( fuv.y ) * ( g0x * textureLod( tex, p2, lod ) + g1x * textureLod( tex, p3, lod ) );
	}
	vec4 textureBicubic( sampler2D sampler, vec2 uv, float lod ) {
		vec2 fLodSize = vec2( textureSize( sampler, int( lod ) ) );
		vec2 cLodSize = vec2( textureSize( sampler, int( lod + 1.0 ) ) );
		vec2 fLodSizeInv = 1.0 / fLodSize;
		vec2 cLodSizeInv = 1.0 / cLodSize;
		vec4 fSample = bicubic( sampler, uv, vec4( fLodSizeInv, fLodSize ), floor( lod ) );
		vec4 cSample = bicubic( sampler, uv, vec4( cLodSizeInv, cLodSize ), ceil( lod ) );
		return mix( fSample, cSample, fract( lod ) );
	}
	vec3 getVolumeTransmissionRay( const in vec3 n, const in vec3 v, const in float thickness, const in float ior, const in mat4 modelMatrix ) {
		vec3 refractionVector = refract( - v, normalize( n ), 1.0 / ior );
		vec3 modelScale;
		modelScale.x = length( vec3( modelMatrix[ 0 ].xyz ) );
		modelScale.y = length( vec3( modelMatrix[ 1 ].xyz ) );
		modelScale.z = length( vec3( modelMatrix[ 2 ].xyz ) );
		return normalize( refractionVector ) * thickness * modelScale;
	}
	float applyIorToRoughness( const in float roughness, const in float ior ) {
		return roughness * clamp( ior * 2.0 - 2.0, 0.0, 1.0 );
	}
	vec4 getTransmissionSample( const in vec2 fragCoord, const in float roughness, const in float ior ) {
		float lod = log2( transmissionSamplerSize.x ) * applyIorToRoughness( roughness, ior );
		return textureBicubic( transmissionSamplerMap, fragCoord.xy, lod );
	}
	vec3 volumeAttenuation( const in float transmissionDistance, const in vec3 attenuationColor, const in float attenuationDistance ) {
		if ( isinf( attenuationDistance ) ) {
			return vec3( 1.0 );
		} else {
			vec3 attenuationCoefficient = -log( attenuationColor ) / attenuationDistance;
			vec3 transmittance = exp( - attenuationCoefficient * transmissionDistance );			return transmittance;
		}
	}
	vec4 getIBLVolumeRefraction( const in vec3 n, const in vec3 v, const in float roughness, const in vec3 diffuseColor,
		const in vec3 specularColor, const in float specularF90, const in vec3 position, const in mat4 modelMatrix,
		const in mat4 viewMatrix, const in mat4 projMatrix, const in float dispersion, const in float ior, const in float thickness,
		const in vec3 attenuationColor, const in float attenuationDistance ) {
		vec4 transmittedLight;
		vec3 transmittance;
		#ifdef USE_DISPERSION
			float halfSpread = ( ior - 1.0 ) * 0.025 * dispersion;
			vec3 iors = vec3( ior - halfSpread, ior, ior + halfSpread );
			for ( int i = 0; i < 3; i ++ ) {
				vec3 transmissionRay = getVolumeTransmissionRay( n, v, thickness, iors[ i ], modelMatrix );
				vec3 refractedRayExit = position + transmissionRay;
		
				vec4 ndcPos = projMatrix * viewMatrix * vec4( refractedRayExit, 1.0 );
				vec2 refractionCoords = ndcPos.xy / ndcPos.w;
				refractionCoords += 1.0;
				refractionCoords /= 2.0;
		
				vec4 transmissionSample = getTransmissionSample( refractionCoords, roughness, iors[ i ] );
				transmittedLight[ i ] = transmissionSample[ i ];
				transmittedLight.a += transmissionSample.a;
				transmittance[ i ] = diffuseColor[ i ] * volumeAttenuation( length( transmissionRay ), attenuationColor, attenuationDistance )[ i ];
			}
			transmittedLight.a /= 3.0;
		
		#else
		
			vec3 transmissionRay = getVolumeTransmissionRay( n, v, thickness, ior, modelMatrix );
			vec3 refractedRayExit = position + transmissionRay;
			vec4 ndcPos = projMatrix * viewMatrix * vec4( refractedRayExit, 1.0 );
			vec2 refractionCoords = ndcPos.xy / ndcPos.w;
			refractionCoords += 1.0;
			refractionCoords /= 2.0;
			transmittedLight = getTransmissionSample( refractionCoords, roughness, ior );
			transmittance = diffuseColor * volumeAttenuation( length( transmissionRay ), attenuationColor, attenuationDistance );
		
		#endif
		vec3 attenuatedColor = transmittance * transmittedLight.rgb;
		vec3 F = EnvironmentBRDF( n, v, specularColor, specularF90, roughness );
		float transmittanceFactor = ( transmittance.r + transmittance.g + transmittance.b ) / 3.0;
		return vec4( ( 1.0 - F ) * attenuatedColor, 1.0 - ( 1.0 - transmittedLight.a ) * transmittanceFactor );
	}
#endif`,HE=`#if defined( USE_UV ) || defined( USE_ANISOTROPY )
	varying vec2 vUv;
#endif
#ifdef USE_MAP
	varying vec2 vMapUv;
#endif
#ifdef USE_ALPHAMAP
	varying vec2 vAlphaMapUv;
#endif
#ifdef USE_LIGHTMAP
	varying vec2 vLightMapUv;
#endif
#ifdef USE_AOMAP
	varying vec2 vAoMapUv;
#endif
#ifdef USE_BUMPMAP
	varying vec2 vBumpMapUv;
#endif
#ifdef USE_NORMALMAP
	varying vec2 vNormalMapUv;
#endif
#ifdef USE_EMISSIVEMAP
	varying vec2 vEmissiveMapUv;
#endif
#ifdef USE_METALNESSMAP
	varying vec2 vMetalnessMapUv;
#endif
#ifdef USE_ROUGHNESSMAP
	varying vec2 vRoughnessMapUv;
#endif
#ifdef USE_ANISOTROPYMAP
	varying vec2 vAnisotropyMapUv;
#endif
#ifdef USE_CLEARCOATMAP
	varying vec2 vClearcoatMapUv;
#endif
#ifdef USE_CLEARCOAT_NORMALMAP
	varying vec2 vClearcoatNormalMapUv;
#endif
#ifdef USE_CLEARCOAT_ROUGHNESSMAP
	varying vec2 vClearcoatRoughnessMapUv;
#endif
#ifdef USE_IRIDESCENCEMAP
	varying vec2 vIridescenceMapUv;
#endif
#ifdef USE_IRIDESCENCE_THICKNESSMAP
	varying vec2 vIridescenceThicknessMapUv;
#endif
#ifdef USE_SHEEN_COLORMAP
	varying vec2 vSheenColorMapUv;
#endif
#ifdef USE_SHEEN_ROUGHNESSMAP
	varying vec2 vSheenRoughnessMapUv;
#endif
#ifdef USE_SPECULARMAP
	varying vec2 vSpecularMapUv;
#endif
#ifdef USE_SPECULAR_COLORMAP
	varying vec2 vSpecularColorMapUv;
#endif
#ifdef USE_SPECULAR_INTENSITYMAP
	varying vec2 vSpecularIntensityMapUv;
#endif
#ifdef USE_TRANSMISSIONMAP
	uniform mat3 transmissionMapTransform;
	varying vec2 vTransmissionMapUv;
#endif
#ifdef USE_THICKNESSMAP
	uniform mat3 thicknessMapTransform;
	varying vec2 vThicknessMapUv;
#endif`,VE=`#if defined( USE_UV ) || defined( USE_ANISOTROPY )
	varying vec2 vUv;
#endif
#ifdef USE_MAP
	uniform mat3 mapTransform;
	varying vec2 vMapUv;
#endif
#ifdef USE_ALPHAMAP
	uniform mat3 alphaMapTransform;
	varying vec2 vAlphaMapUv;
#endif
#ifdef USE_LIGHTMAP
	uniform mat3 lightMapTransform;
	varying vec2 vLightMapUv;
#endif
#ifdef USE_AOMAP
	uniform mat3 aoMapTransform;
	varying vec2 vAoMapUv;
#endif
#ifdef USE_BUMPMAP
	uniform mat3 bumpMapTransform;
	varying vec2 vBumpMapUv;
#endif
#ifdef USE_NORMALMAP
	uniform mat3 normalMapTransform;
	varying vec2 vNormalMapUv;
#endif
#ifdef USE_DISPLACEMENTMAP
	uniform mat3 displacementMapTransform;
	varying vec2 vDisplacementMapUv;
#endif
#ifdef USE_EMISSIVEMAP
	uniform mat3 emissiveMapTransform;
	varying vec2 vEmissiveMapUv;
#endif
#ifdef USE_METALNESSMAP
	uniform mat3 metalnessMapTransform;
	varying vec2 vMetalnessMapUv;
#endif
#ifdef USE_ROUGHNESSMAP
	uniform mat3 roughnessMapTransform;
	varying vec2 vRoughnessMapUv;
#endif
#ifdef USE_ANISOTROPYMAP
	uniform mat3 anisotropyMapTransform;
	varying vec2 vAnisotropyMapUv;
#endif
#ifdef USE_CLEARCOATMAP
	uniform mat3 clearcoatMapTransform;
	varying vec2 vClearcoatMapUv;
#endif
#ifdef USE_CLEARCOAT_NORMALMAP
	uniform mat3 clearcoatNormalMapTransform;
	varying vec2 vClearcoatNormalMapUv;
#endif
#ifdef USE_CLEARCOAT_ROUGHNESSMAP
	uniform mat3 clearcoatRoughnessMapTransform;
	varying vec2 vClearcoatRoughnessMapUv;
#endif
#ifdef USE_SHEEN_COLORMAP
	uniform mat3 sheenColorMapTransform;
	varying vec2 vSheenColorMapUv;
#endif
#ifdef USE_SHEEN_ROUGHNESSMAP
	uniform mat3 sheenRoughnessMapTransform;
	varying vec2 vSheenRoughnessMapUv;
#endif
#ifdef USE_IRIDESCENCEMAP
	uniform mat3 iridescenceMapTransform;
	varying vec2 vIridescenceMapUv;
#endif
#ifdef USE_IRIDESCENCE_THICKNESSMAP
	uniform mat3 iridescenceThicknessMapTransform;
	varying vec2 vIridescenceThicknessMapUv;
#endif
#ifdef USE_SPECULARMAP
	uniform mat3 specularMapTransform;
	varying vec2 vSpecularMapUv;
#endif
#ifdef USE_SPECULAR_COLORMAP
	uniform mat3 specularColorMapTransform;
	varying vec2 vSpecularColorMapUv;
#endif
#ifdef USE_SPECULAR_INTENSITYMAP
	uniform mat3 specularIntensityMapTransform;
	varying vec2 vSpecularIntensityMapUv;
#endif
#ifdef USE_TRANSMISSIONMAP
	uniform mat3 transmissionMapTransform;
	varying vec2 vTransmissionMapUv;
#endif
#ifdef USE_THICKNESSMAP
	uniform mat3 thicknessMapTransform;
	varying vec2 vThicknessMapUv;
#endif`,GE=`#if defined( USE_UV ) || defined( USE_ANISOTROPY )
	vUv = vec3( uv, 1 ).xy;
#endif
#ifdef USE_MAP
	vMapUv = ( mapTransform * vec3( MAP_UV, 1 ) ).xy;
#endif
#ifdef USE_ALPHAMAP
	vAlphaMapUv = ( alphaMapTransform * vec3( ALPHAMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_LIGHTMAP
	vLightMapUv = ( lightMapTransform * vec3( LIGHTMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_AOMAP
	vAoMapUv = ( aoMapTransform * vec3( AOMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_BUMPMAP
	vBumpMapUv = ( bumpMapTransform * vec3( BUMPMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_NORMALMAP
	vNormalMapUv = ( normalMapTransform * vec3( NORMALMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_DISPLACEMENTMAP
	vDisplacementMapUv = ( displacementMapTransform * vec3( DISPLACEMENTMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_EMISSIVEMAP
	vEmissiveMapUv = ( emissiveMapTransform * vec3( EMISSIVEMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_METALNESSMAP
	vMetalnessMapUv = ( metalnessMapTransform * vec3( METALNESSMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_ROUGHNESSMAP
	vRoughnessMapUv = ( roughnessMapTransform * vec3( ROUGHNESSMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_ANISOTROPYMAP
	vAnisotropyMapUv = ( anisotropyMapTransform * vec3( ANISOTROPYMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_CLEARCOATMAP
	vClearcoatMapUv = ( clearcoatMapTransform * vec3( CLEARCOATMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_CLEARCOAT_NORMALMAP
	vClearcoatNormalMapUv = ( clearcoatNormalMapTransform * vec3( CLEARCOAT_NORMALMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_CLEARCOAT_ROUGHNESSMAP
	vClearcoatRoughnessMapUv = ( clearcoatRoughnessMapTransform * vec3( CLEARCOAT_ROUGHNESSMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_IRIDESCENCEMAP
	vIridescenceMapUv = ( iridescenceMapTransform * vec3( IRIDESCENCEMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_IRIDESCENCE_THICKNESSMAP
	vIridescenceThicknessMapUv = ( iridescenceThicknessMapTransform * vec3( IRIDESCENCE_THICKNESSMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_SHEEN_COLORMAP
	vSheenColorMapUv = ( sheenColorMapTransform * vec3( SHEEN_COLORMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_SHEEN_ROUGHNESSMAP
	vSheenRoughnessMapUv = ( sheenRoughnessMapTransform * vec3( SHEEN_ROUGHNESSMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_SPECULARMAP
	vSpecularMapUv = ( specularMapTransform * vec3( SPECULARMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_SPECULAR_COLORMAP
	vSpecularColorMapUv = ( specularColorMapTransform * vec3( SPECULAR_COLORMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_SPECULAR_INTENSITYMAP
	vSpecularIntensityMapUv = ( specularIntensityMapTransform * vec3( SPECULAR_INTENSITYMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_TRANSMISSIONMAP
	vTransmissionMapUv = ( transmissionMapTransform * vec3( TRANSMISSIONMAP_UV, 1 ) ).xy;
#endif
#ifdef USE_THICKNESSMAP
	vThicknessMapUv = ( thicknessMapTransform * vec3( THICKNESSMAP_UV, 1 ) ).xy;
#endif`,WE=`#if defined( USE_ENVMAP ) || defined( DISTANCE ) || defined ( USE_SHADOWMAP ) || defined ( USE_TRANSMISSION ) || NUM_SPOT_LIGHT_COORDS > 0
	vec4 worldPosition = vec4( transformed, 1.0 );
	#ifdef USE_BATCHING
		worldPosition = batchingMatrix * worldPosition;
	#endif
	#ifdef USE_INSTANCING
		worldPosition = instanceMatrix * worldPosition;
	#endif
	worldPosition = modelMatrix * worldPosition;
#endif`;const XE=`varying vec2 vUv;
uniform mat3 uvTransform;
void main() {
	vUv = ( uvTransform * vec3( uv, 1 ) ).xy;
	gl_Position = vec4( position.xy, 1.0, 1.0 );
}`,jE=`uniform sampler2D t2D;
uniform float backgroundIntensity;
varying vec2 vUv;
void main() {
	vec4 texColor = texture2D( t2D, vUv );
	#ifdef DECODE_VIDEO_TEXTURE
		texColor = vec4( mix( pow( texColor.rgb * 0.9478672986 + vec3( 0.0521327014 ), vec3( 2.4 ) ), texColor.rgb * 0.0773993808, vec3( lessThanEqual( texColor.rgb, vec3( 0.04045 ) ) ) ), texColor.w );
	#endif
	texColor.rgb *= backgroundIntensity;
	gl_FragColor = texColor;
	#include <tonemapping_fragment>
	#include <colorspace_fragment>
}`,qE=`varying vec3 vWorldDirection;
#include <common>
void main() {
	vWorldDirection = transformDirection( position, modelMatrix );
	#include <begin_vertex>
	#include <project_vertex>
	gl_Position.z = gl_Position.w;
}`,YE=`#ifdef ENVMAP_TYPE_CUBE
	uniform samplerCube envMap;
#elif defined( ENVMAP_TYPE_CUBE_UV )
	uniform sampler2D envMap;
#endif
uniform float flipEnvMap;
uniform float backgroundBlurriness;
uniform float backgroundIntensity;
uniform mat3 backgroundRotation;
varying vec3 vWorldDirection;
#include <cube_uv_reflection_fragment>
void main() {
	#ifdef ENVMAP_TYPE_CUBE
		vec4 texColor = textureCube( envMap, backgroundRotation * vec3( flipEnvMap * vWorldDirection.x, vWorldDirection.yz ) );
	#elif defined( ENVMAP_TYPE_CUBE_UV )
		vec4 texColor = textureCubeUV( envMap, backgroundRotation * vWorldDirection, backgroundBlurriness );
	#else
		vec4 texColor = vec4( 0.0, 0.0, 0.0, 1.0 );
	#endif
	texColor.rgb *= backgroundIntensity;
	gl_FragColor = texColor;
	#include <tonemapping_fragment>
	#include <colorspace_fragment>
}`,KE=`varying vec3 vWorldDirection;
#include <common>
void main() {
	vWorldDirection = transformDirection( position, modelMatrix );
	#include <begin_vertex>
	#include <project_vertex>
	gl_Position.z = gl_Position.w;
}`,$E=`uniform samplerCube tCube;
uniform float tFlip;
uniform float opacity;
varying vec3 vWorldDirection;
void main() {
	vec4 texColor = textureCube( tCube, vec3( tFlip * vWorldDirection.x, vWorldDirection.yz ) );
	gl_FragColor = texColor;
	gl_FragColor.a *= opacity;
	#include <tonemapping_fragment>
	#include <colorspace_fragment>
}`,ZE=`#include <common>
#include <batching_pars_vertex>
#include <uv_pars_vertex>
#include <displacementmap_pars_vertex>
#include <morphtarget_pars_vertex>
#include <skinning_pars_vertex>
#include <logdepthbuf_pars_vertex>
#include <clipping_planes_pars_vertex>
varying vec2 vHighPrecisionZW;
void main() {
	#include <uv_vertex>
	#include <batching_vertex>
	#include <skinbase_vertex>
	#include <morphinstance_vertex>
	#ifdef USE_DISPLACEMENTMAP
		#include <beginnormal_vertex>
		#include <morphnormal_vertex>
		#include <skinnormal_vertex>
	#endif
	#include <begin_vertex>
	#include <morphtarget_vertex>
	#include <skinning_vertex>
	#include <displacementmap_vertex>
	#include <project_vertex>
	#include <logdepthbuf_vertex>
	#include <clipping_planes_vertex>
	vHighPrecisionZW = gl_Position.zw;
}`,JE=`#if DEPTH_PACKING == 3200
	uniform float opacity;
#endif
#include <common>
#include <packing>
#include <uv_pars_fragment>
#include <map_pars_fragment>
#include <alphamap_pars_fragment>
#include <alphatest_pars_fragment>
#include <alphahash_pars_fragment>
#include <logdepthbuf_pars_fragment>
#include <clipping_planes_pars_fragment>
varying vec2 vHighPrecisionZW;
void main() {
	vec4 diffuseColor = vec4( 1.0 );
	#include <clipping_planes_fragment>
	#if DEPTH_PACKING == 3200
		diffuseColor.a = opacity;
	#endif
	#include <map_fragment>
	#include <alphamap_fragment>
	#include <alphatest_fragment>
	#include <alphahash_fragment>
	#include <logdepthbuf_fragment>
	float fragCoordZ = 0.5 * vHighPrecisionZW[0] / vHighPrecisionZW[1] + 0.5;
	#if DEPTH_PACKING == 3200
		gl_FragColor = vec4( vec3( 1.0 - fragCoordZ ), opacity );
	#elif DEPTH_PACKING == 3201
		gl_FragColor = packDepthToRGBA( fragCoordZ );
	#elif DEPTH_PACKING == 3202
		gl_FragColor = vec4( packDepthToRGB( fragCoordZ ), 1.0 );
	#elif DEPTH_PACKING == 3203
		gl_FragColor = vec4( packDepthToRG( fragCoordZ ), 0.0, 1.0 );
	#endif
}`,QE=`#define DISTANCE
varying vec3 vWorldPosition;
#include <common>
#include <batching_pars_vertex>
#include <uv_pars_vertex>
#include <displacementmap_pars_vertex>
#include <morphtarget_pars_vertex>
#include <skinning_pars_vertex>
#include <clipping_planes_pars_vertex>
void main() {
	#include <uv_vertex>
	#include <batching_vertex>
	#include <skinbase_vertex>
	#include <morphinstance_vertex>
	#ifdef USE_DISPLACEMENTMAP
		#include <beginnormal_vertex>
		#include <morphnormal_vertex>
		#include <skinnormal_vertex>
	#endif
	#include <begin_vertex>
	#include <morphtarget_vertex>
	#include <skinning_vertex>
	#include <displacementmap_vertex>
	#include <project_vertex>
	#include <worldpos_vertex>
	#include <clipping_planes_vertex>
	vWorldPosition = worldPosition.xyz;
}`,e1=`#define DISTANCE
uniform vec3 referencePosition;
uniform float nearDistance;
uniform float farDistance;
varying vec3 vWorldPosition;
#include <common>
#include <packing>
#include <uv_pars_fragment>
#include <map_pars_fragment>
#include <alphamap_pars_fragment>
#include <alphatest_pars_fragment>
#include <alphahash_pars_fragment>
#include <clipping_planes_pars_fragment>
void main () {
	vec4 diffuseColor = vec4( 1.0 );
	#include <clipping_planes_fragment>
	#include <map_fragment>
	#include <alphamap_fragment>
	#include <alphatest_fragment>
	#include <alphahash_fragment>
	float dist = length( vWorldPosition - referencePosition );
	dist = ( dist - nearDistance ) / ( farDistance - nearDistance );
	dist = saturate( dist );
	gl_FragColor = packDepthToRGBA( dist );
}`,t1=`varying vec3 vWorldDirection;
#include <common>
void main() {
	vWorldDirection = transformDirection( position, modelMatrix );
	#include <begin_vertex>
	#include <project_vertex>
}`,n1=`uniform sampler2D tEquirect;
varying vec3 vWorldDirection;
#include <common>
void main() {
	vec3 direction = normalize( vWorldDirection );
	vec2 sampleUV = equirectUv( direction );
	gl_FragColor = texture2D( tEquirect, sampleUV );
	#include <tonemapping_fragment>
	#include <colorspace_fragment>
}`,i1=`uniform float scale;
attribute float lineDistance;
varying float vLineDistance;
#include <common>
#include <uv_pars_vertex>
#include <color_pars_vertex>
#include <fog_pars_vertex>
#include <morphtarget_pars_vertex>
#include <logdepthbuf_pars_vertex>
#include <clipping_planes_pars_vertex>
void main() {
	vLineDistance = scale * lineDistance;
	#include <uv_vertex>
	#include <color_vertex>
	#include <morphinstance_vertex>
	#include <morphcolor_vertex>
	#include <begin_vertex>
	#include <morphtarget_vertex>
	#include <project_vertex>
	#include <logdepthbuf_vertex>
	#include <clipping_planes_vertex>
	#include <fog_vertex>
}`,r1=`uniform vec3 diffuse;
uniform float opacity;
uniform float dashSize;
uniform float totalSize;
varying float vLineDistance;
#include <common>
#include <color_pars_fragment>
#include <uv_pars_fragment>
#include <map_pars_fragment>
#include <fog_pars_fragment>
#include <logdepthbuf_pars_fragment>
#include <clipping_planes_pars_fragment>
void main() {
	vec4 diffuseColor = vec4( diffuse, opacity );
	#include <clipping_planes_fragment>
	if ( mod( vLineDistance, totalSize ) > dashSize ) {
		discard;
	}
	vec3 outgoingLight = vec3( 0.0 );
	#include <logdepthbuf_fragment>
	#include <map_fragment>
	#include <color_fragment>
	outgoingLight = diffuseColor.rgb;
	#include <opaque_fragment>
	#include <tonemapping_fragment>
	#include <colorspace_fragment>
	#include <fog_fragment>
	#include <premultiplied_alpha_fragment>
}`,s1=`#include <common>
#include <batching_pars_vertex>
#include <uv_pars_vertex>
#include <envmap_pars_vertex>
#include <color_pars_vertex>
#include <fog_pars_vertex>
#include <morphtarget_pars_vertex>
#include <skinning_pars_vertex>
#include <logdepthbuf_pars_vertex>
#include <clipping_planes_pars_vertex>
void main() {
	#include <uv_vertex>
	#include <color_vertex>
	#include <morphinstance_vertex>
	#include <morphcolor_vertex>
	#include <batching_vertex>
	#if defined ( USE_ENVMAP ) || defined ( USE_SKINNING )
		#include <beginnormal_vertex>
		#include <morphnormal_vertex>
		#include <skinbase_vertex>
		#include <skinnormal_vertex>
		#include <defaultnormal_vertex>
	#endif
	#include <begin_vertex>
	#include <morphtarget_vertex>
	#include <skinning_vertex>
	#include <project_vertex>
	#include <logdepthbuf_vertex>
	#include <clipping_planes_vertex>
	#include <worldpos_vertex>
	#include <envmap_vertex>
	#include <fog_vertex>
}`,o1=`uniform vec3 diffuse;
uniform float opacity;
#ifndef FLAT_SHADED
	varying vec3 vNormal;
#endif
#include <common>
#include <dithering_pars_fragment>
#include <color_pars_fragment>
#include <uv_pars_fragment>
#include <map_pars_fragment>
#include <alphamap_pars_fragment>
#include <alphatest_pars_fragment>
#include <alphahash_pars_fragment>
#include <aomap_pars_fragment>
#include <lightmap_pars_fragment>
#include <envmap_common_pars_fragment>
#include <envmap_pars_fragment>
#include <fog_pars_fragment>
#include <specularmap_pars_fragment>
#include <logdepthbuf_pars_fragment>
#include <clipping_planes_pars_fragment>
void main() {
	vec4 diffuseColor = vec4( diffuse, opacity );
	#include <clipping_planes_fragment>
	#include <logdepthbuf_fragment>
	#include <map_fragment>
	#include <color_fragment>
	#include <alphamap_fragment>
	#include <alphatest_fragment>
	#include <alphahash_fragment>
	#include <specularmap_fragment>
	ReflectedLight reflectedLight = ReflectedLight( vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ) );
	#ifdef USE_LIGHTMAP
		vec4 lightMapTexel = texture2D( lightMap, vLightMapUv );
		reflectedLight.indirectDiffuse += lightMapTexel.rgb * lightMapIntensity * RECIPROCAL_PI;
	#else
		reflectedLight.indirectDiffuse += vec3( 1.0 );
	#endif
	#include <aomap_fragment>
	reflectedLight.indirectDiffuse *= diffuseColor.rgb;
	vec3 outgoingLight = reflectedLight.indirectDiffuse;
	#include <envmap_fragment>
	#include <opaque_fragment>
	#include <tonemapping_fragment>
	#include <colorspace_fragment>
	#include <fog_fragment>
	#include <premultiplied_alpha_fragment>
	#include <dithering_fragment>
}`,a1=`#define LAMBERT
varying vec3 vViewPosition;
#include <common>
#include <batching_pars_vertex>
#include <uv_pars_vertex>
#include <displacementmap_pars_vertex>
#include <envmap_pars_vertex>
#include <color_pars_vertex>
#include <fog_pars_vertex>
#include <normal_pars_vertex>
#include <morphtarget_pars_vertex>
#include <skinning_pars_vertex>
#include <shadowmap_pars_vertex>
#include <logdepthbuf_pars_vertex>
#include <clipping_planes_pars_vertex>
void main() {
	#include <uv_vertex>
	#include <color_vertex>
	#include <morphinstance_vertex>
	#include <morphcolor_vertex>
	#include <batching_vertex>
	#include <beginnormal_vertex>
	#include <morphnormal_vertex>
	#include <skinbase_vertex>
	#include <skinnormal_vertex>
	#include <defaultnormal_vertex>
	#include <normal_vertex>
	#include <begin_vertex>
	#include <morphtarget_vertex>
	#include <skinning_vertex>
	#include <displacementmap_vertex>
	#include <project_vertex>
	#include <logdepthbuf_vertex>
	#include <clipping_planes_vertex>
	vViewPosition = - mvPosition.xyz;
	#include <worldpos_vertex>
	#include <envmap_vertex>
	#include <shadowmap_vertex>
	#include <fog_vertex>
}`,l1=`#define LAMBERT
uniform vec3 diffuse;
uniform vec3 emissive;
uniform float opacity;
#include <common>
#include <packing>
#include <dithering_pars_fragment>
#include <color_pars_fragment>
#include <uv_pars_fragment>
#include <map_pars_fragment>
#include <alphamap_pars_fragment>
#include <alphatest_pars_fragment>
#include <alphahash_pars_fragment>
#include <aomap_pars_fragment>
#include <lightmap_pars_fragment>
#include <emissivemap_pars_fragment>
#include <envmap_common_pars_fragment>
#include <envmap_pars_fragment>
#include <fog_pars_fragment>
#include <bsdfs>
#include <lights_pars_begin>
#include <normal_pars_fragment>
#include <lights_lambert_pars_fragment>
#include <shadowmap_pars_fragment>
#include <bumpmap_pars_fragment>
#include <normalmap_pars_fragment>
#include <specularmap_pars_fragment>
#include <logdepthbuf_pars_fragment>
#include <clipping_planes_pars_fragment>
void main() {
	vec4 diffuseColor = vec4( diffuse, opacity );
	#include <clipping_planes_fragment>
	ReflectedLight reflectedLight = ReflectedLight( vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ) );
	vec3 totalEmissiveRadiance = emissive;
	#include <logdepthbuf_fragment>
	#include <map_fragment>
	#include <color_fragment>
	#include <alphamap_fragment>
	#include <alphatest_fragment>
	#include <alphahash_fragment>
	#include <specularmap_fragment>
	#include <normal_fragment_begin>
	#include <normal_fragment_maps>
	#include <emissivemap_fragment>
	#include <lights_lambert_fragment>
	#include <lights_fragment_begin>
	#include <lights_fragment_maps>
	#include <lights_fragment_end>
	#include <aomap_fragment>
	vec3 outgoingLight = reflectedLight.directDiffuse + reflectedLight.indirectDiffuse + totalEmissiveRadiance;
	#include <envmap_fragment>
	#include <opaque_fragment>
	#include <tonemapping_fragment>
	#include <colorspace_fragment>
	#include <fog_fragment>
	#include <premultiplied_alpha_fragment>
	#include <dithering_fragment>
}`,u1=`#define MATCAP
varying vec3 vViewPosition;
#include <common>
#include <batching_pars_vertex>
#include <uv_pars_vertex>
#include <color_pars_vertex>
#include <displacementmap_pars_vertex>
#include <fog_pars_vertex>
#include <normal_pars_vertex>
#include <morphtarget_pars_vertex>
#include <skinning_pars_vertex>
#include <logdepthbuf_pars_vertex>
#include <clipping_planes_pars_vertex>
void main() {
	#include <uv_vertex>
	#include <color_vertex>
	#include <morphinstance_vertex>
	#include <morphcolor_vertex>
	#include <batching_vertex>
	#include <beginnormal_vertex>
	#include <morphnormal_vertex>
	#include <skinbase_vertex>
	#include <skinnormal_vertex>
	#include <defaultnormal_vertex>
	#include <normal_vertex>
	#include <begin_vertex>
	#include <morphtarget_vertex>
	#include <skinning_vertex>
	#include <displacementmap_vertex>
	#include <project_vertex>
	#include <logdepthbuf_vertex>
	#include <clipping_planes_vertex>
	#include <fog_vertex>
	vViewPosition = - mvPosition.xyz;
}`,c1=`#define MATCAP
uniform vec3 diffuse;
uniform float opacity;
uniform sampler2D matcap;
varying vec3 vViewPosition;
#include <common>
#include <dithering_pars_fragment>
#include <color_pars_fragment>
#include <uv_pars_fragment>
#include <map_pars_fragment>
#include <alphamap_pars_fragment>
#include <alphatest_pars_fragment>
#include <alphahash_pars_fragment>
#include <fog_pars_fragment>
#include <normal_pars_fragment>
#include <bumpmap_pars_fragment>
#include <normalmap_pars_fragment>
#include <logdepthbuf_pars_fragment>
#include <clipping_planes_pars_fragment>
void main() {
	vec4 diffuseColor = vec4( diffuse, opacity );
	#include <clipping_planes_fragment>
	#include <logdepthbuf_fragment>
	#include <map_fragment>
	#include <color_fragment>
	#include <alphamap_fragment>
	#include <alphatest_fragment>
	#include <alphahash_fragment>
	#include <normal_fragment_begin>
	#include <normal_fragment_maps>
	vec3 viewDir = normalize( vViewPosition );
	vec3 x = normalize( vec3( viewDir.z, 0.0, - viewDir.x ) );
	vec3 y = cross( viewDir, x );
	vec2 uv = vec2( dot( x, normal ), dot( y, normal ) ) * 0.495 + 0.5;
	#ifdef USE_MATCAP
		vec4 matcapColor = texture2D( matcap, uv );
	#else
		vec4 matcapColor = vec4( vec3( mix( 0.2, 0.8, uv.y ) ), 1.0 );
	#endif
	vec3 outgoingLight = diffuseColor.rgb * matcapColor.rgb;
	#include <opaque_fragment>
	#include <tonemapping_fragment>
	#include <colorspace_fragment>
	#include <fog_fragment>
	#include <premultiplied_alpha_fragment>
	#include <dithering_fragment>
}`,f1=`#define NORMAL
#if defined( FLAT_SHADED ) || defined( USE_BUMPMAP ) || defined( USE_NORMALMAP_TANGENTSPACE )
	varying vec3 vViewPosition;
#endif
#include <common>
#include <batching_pars_vertex>
#include <uv_pars_vertex>
#include <displacementmap_pars_vertex>
#include <normal_pars_vertex>
#include <morphtarget_pars_vertex>
#include <skinning_pars_vertex>
#include <logdepthbuf_pars_vertex>
#include <clipping_planes_pars_vertex>
void main() {
	#include <uv_vertex>
	#include <batching_vertex>
	#include <beginnormal_vertex>
	#include <morphinstance_vertex>
	#include <morphnormal_vertex>
	#include <skinbase_vertex>
	#include <skinnormal_vertex>
	#include <defaultnormal_vertex>
	#include <normal_vertex>
	#include <begin_vertex>
	#include <morphtarget_vertex>
	#include <skinning_vertex>
	#include <displacementmap_vertex>
	#include <project_vertex>
	#include <logdepthbuf_vertex>
	#include <clipping_planes_vertex>
#if defined( FLAT_SHADED ) || defined( USE_BUMPMAP ) || defined( USE_NORMALMAP_TANGENTSPACE )
	vViewPosition = - mvPosition.xyz;
#endif
}`,h1=`#define NORMAL
uniform float opacity;
#if defined( FLAT_SHADED ) || defined( USE_BUMPMAP ) || defined( USE_NORMALMAP_TANGENTSPACE )
	varying vec3 vViewPosition;
#endif
#include <packing>
#include <uv_pars_fragment>
#include <normal_pars_fragment>
#include <bumpmap_pars_fragment>
#include <normalmap_pars_fragment>
#include <logdepthbuf_pars_fragment>
#include <clipping_planes_pars_fragment>
void main() {
	vec4 diffuseColor = vec4( 0.0, 0.0, 0.0, opacity );
	#include <clipping_planes_fragment>
	#include <logdepthbuf_fragment>
	#include <normal_fragment_begin>
	#include <normal_fragment_maps>
	gl_FragColor = vec4( packNormalToRGB( normal ), diffuseColor.a );
	#ifdef OPAQUE
		gl_FragColor.a = 1.0;
	#endif
}`,d1=`#define PHONG
varying vec3 vViewPosition;
#include <common>
#include <batching_pars_vertex>
#include <uv_pars_vertex>
#include <displacementmap_pars_vertex>
#include <envmap_pars_vertex>
#include <color_pars_vertex>
#include <fog_pars_vertex>
#include <normal_pars_vertex>
#include <morphtarget_pars_vertex>
#include <skinning_pars_vertex>
#include <shadowmap_pars_vertex>
#include <logdepthbuf_pars_vertex>
#include <clipping_planes_pars_vertex>
void main() {
	#include <uv_vertex>
	#include <color_vertex>
	#include <morphcolor_vertex>
	#include <batching_vertex>
	#include <beginnormal_vertex>
	#include <morphinstance_vertex>
	#include <morphnormal_vertex>
	#include <skinbase_vertex>
	#include <skinnormal_vertex>
	#include <defaultnormal_vertex>
	#include <normal_vertex>
	#include <begin_vertex>
	#include <morphtarget_vertex>
	#include <skinning_vertex>
	#include <displacementmap_vertex>
	#include <project_vertex>
	#include <logdepthbuf_vertex>
	#include <clipping_planes_vertex>
	vViewPosition = - mvPosition.xyz;
	#include <worldpos_vertex>
	#include <envmap_vertex>
	#include <shadowmap_vertex>
	#include <fog_vertex>
}`,p1=`#define PHONG
uniform vec3 diffuse;
uniform vec3 emissive;
uniform vec3 specular;
uniform float shininess;
uniform float opacity;
#include <common>
#include <packing>
#include <dithering_pars_fragment>
#include <color_pars_fragment>
#include <uv_pars_fragment>
#include <map_pars_fragment>
#include <alphamap_pars_fragment>
#include <alphatest_pars_fragment>
#include <alphahash_pars_fragment>
#include <aomap_pars_fragment>
#include <lightmap_pars_fragment>
#include <emissivemap_pars_fragment>
#include <envmap_common_pars_fragment>
#include <envmap_pars_fragment>
#include <fog_pars_fragment>
#include <bsdfs>
#include <lights_pars_begin>
#include <normal_pars_fragment>
#include <lights_phong_pars_fragment>
#include <shadowmap_pars_fragment>
#include <bumpmap_pars_fragment>
#include <normalmap_pars_fragment>
#include <specularmap_pars_fragment>
#include <logdepthbuf_pars_fragment>
#include <clipping_planes_pars_fragment>
void main() {
	vec4 diffuseColor = vec4( diffuse, opacity );
	#include <clipping_planes_fragment>
	ReflectedLight reflectedLight = ReflectedLight( vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ) );
	vec3 totalEmissiveRadiance = emissive;
	#include <logdepthbuf_fragment>
	#include <map_fragment>
	#include <color_fragment>
	#include <alphamap_fragment>
	#include <alphatest_fragment>
	#include <alphahash_fragment>
	#include <specularmap_fragment>
	#include <normal_fragment_begin>
	#include <normal_fragment_maps>
	#include <emissivemap_fragment>
	#include <lights_phong_fragment>
	#include <lights_fragment_begin>
	#include <lights_fragment_maps>
	#include <lights_fragment_end>
	#include <aomap_fragment>
	vec3 outgoingLight = reflectedLight.directDiffuse + reflectedLight.indirectDiffuse + reflectedLight.directSpecular + reflectedLight.indirectSpecular + totalEmissiveRadiance;
	#include <envmap_fragment>
	#include <opaque_fragment>
	#include <tonemapping_fragment>
	#include <colorspace_fragment>
	#include <fog_fragment>
	#include <premultiplied_alpha_fragment>
	#include <dithering_fragment>
}`,m1=`#define STANDARD
varying vec3 vViewPosition;
#ifdef USE_TRANSMISSION
	varying vec3 vWorldPosition;
#endif
#include <common>
#include <batching_pars_vertex>
#include <uv_pars_vertex>
#include <displacementmap_pars_vertex>
#include <color_pars_vertex>
#include <fog_pars_vertex>
#include <normal_pars_vertex>
#include <morphtarget_pars_vertex>
#include <skinning_pars_vertex>
#include <shadowmap_pars_vertex>
#include <logdepthbuf_pars_vertex>
#include <clipping_planes_pars_vertex>
void main() {
	#include <uv_vertex>
	#include <color_vertex>
	#include <morphinstance_vertex>
	#include <morphcolor_vertex>
	#include <batching_vertex>
	#include <beginnormal_vertex>
	#include <morphnormal_vertex>
	#include <skinbase_vertex>
	#include <skinnormal_vertex>
	#include <defaultnormal_vertex>
	#include <normal_vertex>
	#include <begin_vertex>
	#include <morphtarget_vertex>
	#include <skinning_vertex>
	#include <displacementmap_vertex>
	#include <project_vertex>
	#include <logdepthbuf_vertex>
	#include <clipping_planes_vertex>
	vViewPosition = - mvPosition.xyz;
	#include <worldpos_vertex>
	#include <shadowmap_vertex>
	#include <fog_vertex>
#ifdef USE_TRANSMISSION
	vWorldPosition = worldPosition.xyz;
#endif
}`,g1=`#define STANDARD
#ifdef PHYSICAL
	#define IOR
	#define USE_SPECULAR
#endif
uniform vec3 diffuse;
uniform vec3 emissive;
uniform float roughness;
uniform float metalness;
uniform float opacity;
#ifdef IOR
	uniform float ior;
#endif
#ifdef USE_SPECULAR
	uniform float specularIntensity;
	uniform vec3 specularColor;
	#ifdef USE_SPECULAR_COLORMAP
		uniform sampler2D specularColorMap;
	#endif
	#ifdef USE_SPECULAR_INTENSITYMAP
		uniform sampler2D specularIntensityMap;
	#endif
#endif
#ifdef USE_CLEARCOAT
	uniform float clearcoat;
	uniform float clearcoatRoughness;
#endif
#ifdef USE_DISPERSION
	uniform float dispersion;
#endif
#ifdef USE_IRIDESCENCE
	uniform float iridescence;
	uniform float iridescenceIOR;
	uniform float iridescenceThicknessMinimum;
	uniform float iridescenceThicknessMaximum;
#endif
#ifdef USE_SHEEN
	uniform vec3 sheenColor;
	uniform float sheenRoughness;
	#ifdef USE_SHEEN_COLORMAP
		uniform sampler2D sheenColorMap;
	#endif
	#ifdef USE_SHEEN_ROUGHNESSMAP
		uniform sampler2D sheenRoughnessMap;
	#endif
#endif
#ifdef USE_ANISOTROPY
	uniform vec2 anisotropyVector;
	#ifdef USE_ANISOTROPYMAP
		uniform sampler2D anisotropyMap;
	#endif
#endif
varying vec3 vViewPosition;
#include <common>
#include <packing>
#include <dithering_pars_fragment>
#include <color_pars_fragment>
#include <uv_pars_fragment>
#include <map_pars_fragment>
#include <alphamap_pars_fragment>
#include <alphatest_pars_fragment>
#include <alphahash_pars_fragment>
#include <aomap_pars_fragment>
#include <lightmap_pars_fragment>
#include <emissivemap_pars_fragment>
#include <iridescence_fragment>
#include <cube_uv_reflection_fragment>
#include <envmap_common_pars_fragment>
#include <envmap_physical_pars_fragment>
#include <fog_pars_fragment>
#include <lights_pars_begin>
#include <normal_pars_fragment>
#include <lights_physical_pars_fragment>
#include <transmission_pars_fragment>
#include <shadowmap_pars_fragment>
#include <bumpmap_pars_fragment>
#include <normalmap_pars_fragment>
#include <clearcoat_pars_fragment>
#include <iridescence_pars_fragment>
#include <roughnessmap_pars_fragment>
#include <metalnessmap_pars_fragment>
#include <logdepthbuf_pars_fragment>
#include <clipping_planes_pars_fragment>
void main() {
	vec4 diffuseColor = vec4( diffuse, opacity );
	#include <clipping_planes_fragment>
	ReflectedLight reflectedLight = ReflectedLight( vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ) );
	vec3 totalEmissiveRadiance = emissive;
	#include <logdepthbuf_fragment>
	#include <map_fragment>
	#include <color_fragment>
	#include <alphamap_fragment>
	#include <alphatest_fragment>
	#include <alphahash_fragment>
	#include <roughnessmap_fragment>
	#include <metalnessmap_fragment>
	#include <normal_fragment_begin>
	#include <normal_fragment_maps>
	#include <clearcoat_normal_fragment_begin>
	#include <clearcoat_normal_fragment_maps>
	#include <emissivemap_fragment>
	#include <lights_physical_fragment>
	#include <lights_fragment_begin>
	#include <lights_fragment_maps>
	#include <lights_fragment_end>
	#include <aomap_fragment>
	vec3 totalDiffuse = reflectedLight.directDiffuse + reflectedLight.indirectDiffuse;
	vec3 totalSpecular = reflectedLight.directSpecular + reflectedLight.indirectSpecular;
	#include <transmission_fragment>
	vec3 outgoingLight = totalDiffuse + totalSpecular + totalEmissiveRadiance;
	#ifdef USE_SHEEN
		float sheenEnergyComp = 1.0 - 0.157 * max3( material.sheenColor );
		outgoingLight = outgoingLight * sheenEnergyComp + sheenSpecularDirect + sheenSpecularIndirect;
	#endif
	#ifdef USE_CLEARCOAT
		float dotNVcc = saturate( dot( geometryClearcoatNormal, geometryViewDir ) );
		vec3 Fcc = F_Schlick( material.clearcoatF0, material.clearcoatF90, dotNVcc );
		outgoingLight = outgoingLight * ( 1.0 - material.clearcoat * Fcc ) + ( clearcoatSpecularDirect + clearcoatSpecularIndirect ) * material.clearcoat;
	#endif
	#include <opaque_fragment>
	#include <tonemapping_fragment>
	#include <colorspace_fragment>
	#include <fog_fragment>
	#include <premultiplied_alpha_fragment>
	#include <dithering_fragment>
}`,_1=`#define TOON
varying vec3 vViewPosition;
#include <common>
#include <batching_pars_vertex>
#include <uv_pars_vertex>
#include <displacementmap_pars_vertex>
#include <color_pars_vertex>
#include <fog_pars_vertex>
#include <normal_pars_vertex>
#include <morphtarget_pars_vertex>
#include <skinning_pars_vertex>
#include <shadowmap_pars_vertex>
#include <logdepthbuf_pars_vertex>
#include <clipping_planes_pars_vertex>
void main() {
	#include <uv_vertex>
	#include <color_vertex>
	#include <morphinstance_vertex>
	#include <morphcolor_vertex>
	#include <batching_vertex>
	#include <beginnormal_vertex>
	#include <morphnormal_vertex>
	#include <skinbase_vertex>
	#include <skinnormal_vertex>
	#include <defaultnormal_vertex>
	#include <normal_vertex>
	#include <begin_vertex>
	#include <morphtarget_vertex>
	#include <skinning_vertex>
	#include <displacementmap_vertex>
	#include <project_vertex>
	#include <logdepthbuf_vertex>
	#include <clipping_planes_vertex>
	vViewPosition = - mvPosition.xyz;
	#include <worldpos_vertex>
	#include <shadowmap_vertex>
	#include <fog_vertex>
}`,v1=`#define TOON
uniform vec3 diffuse;
uniform vec3 emissive;
uniform float opacity;
#include <common>
#include <packing>
#include <dithering_pars_fragment>
#include <color_pars_fragment>
#include <uv_pars_fragment>
#include <map_pars_fragment>
#include <alphamap_pars_fragment>
#include <alphatest_pars_fragment>
#include <alphahash_pars_fragment>
#include <aomap_pars_fragment>
#include <lightmap_pars_fragment>
#include <emissivemap_pars_fragment>
#include <gradientmap_pars_fragment>
#include <fog_pars_fragment>
#include <bsdfs>
#include <lights_pars_begin>
#include <normal_pars_fragment>
#include <lights_toon_pars_fragment>
#include <shadowmap_pars_fragment>
#include <bumpmap_pars_fragment>
#include <normalmap_pars_fragment>
#include <logdepthbuf_pars_fragment>
#include <clipping_planes_pars_fragment>
void main() {
	vec4 diffuseColor = vec4( diffuse, opacity );
	#include <clipping_planes_fragment>
	ReflectedLight reflectedLight = ReflectedLight( vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ) );
	vec3 totalEmissiveRadiance = emissive;
	#include <logdepthbuf_fragment>
	#include <map_fragment>
	#include <color_fragment>
	#include <alphamap_fragment>
	#include <alphatest_fragment>
	#include <alphahash_fragment>
	#include <normal_fragment_begin>
	#include <normal_fragment_maps>
	#include <emissivemap_fragment>
	#include <lights_toon_fragment>
	#include <lights_fragment_begin>
	#include <lights_fragment_maps>
	#include <lights_fragment_end>
	#include <aomap_fragment>
	vec3 outgoingLight = reflectedLight.directDiffuse + reflectedLight.indirectDiffuse + totalEmissiveRadiance;
	#include <opaque_fragment>
	#include <tonemapping_fragment>
	#include <colorspace_fragment>
	#include <fog_fragment>
	#include <premultiplied_alpha_fragment>
	#include <dithering_fragment>
}`,y1=`uniform float size;
uniform float scale;
#include <common>
#include <color_pars_vertex>
#include <fog_pars_vertex>
#include <morphtarget_pars_vertex>
#include <logdepthbuf_pars_vertex>
#include <clipping_planes_pars_vertex>
#ifdef USE_POINTS_UV
	varying vec2 vUv;
	uniform mat3 uvTransform;
#endif
void main() {
	#ifdef USE_POINTS_UV
		vUv = ( uvTransform * vec3( uv, 1 ) ).xy;
	#endif
	#include <color_vertex>
	#include <morphinstance_vertex>
	#include <morphcolor_vertex>
	#include <begin_vertex>
	#include <morphtarget_vertex>
	#include <project_vertex>
	gl_PointSize = size;
	#ifdef USE_SIZEATTENUATION
		bool isPerspective = isPerspectiveMatrix( projectionMatrix );
		if ( isPerspective ) gl_PointSize *= ( scale / - mvPosition.z );
	#endif
	#include <logdepthbuf_vertex>
	#include <clipping_planes_vertex>
	#include <worldpos_vertex>
	#include <fog_vertex>
}`,x1=`uniform vec3 diffuse;
uniform float opacity;
#include <common>
#include <color_pars_fragment>
#include <map_particle_pars_fragment>
#include <alphatest_pars_fragment>
#include <alphahash_pars_fragment>
#include <fog_pars_fragment>
#include <logdepthbuf_pars_fragment>
#include <clipping_planes_pars_fragment>
void main() {
	vec4 diffuseColor = vec4( diffuse, opacity );
	#include <clipping_planes_fragment>
	vec3 outgoingLight = vec3( 0.0 );
	#include <logdepthbuf_fragment>
	#include <map_particle_fragment>
	#include <color_fragment>
	#include <alphatest_fragment>
	#include <alphahash_fragment>
	outgoingLight = diffuseColor.rgb;
	#include <opaque_fragment>
	#include <tonemapping_fragment>
	#include <colorspace_fragment>
	#include <fog_fragment>
	#include <premultiplied_alpha_fragment>
}`,S1=`#include <common>
#include <batching_pars_vertex>
#include <fog_pars_vertex>
#include <morphtarget_pars_vertex>
#include <skinning_pars_vertex>
#include <logdepthbuf_pars_vertex>
#include <shadowmap_pars_vertex>
void main() {
	#include <batching_vertex>
	#include <beginnormal_vertex>
	#include <morphinstance_vertex>
	#include <morphnormal_vertex>
	#include <skinbase_vertex>
	#include <skinnormal_vertex>
	#include <defaultnormal_vertex>
	#include <begin_vertex>
	#include <morphtarget_vertex>
	#include <skinning_vertex>
	#include <project_vertex>
	#include <logdepthbuf_vertex>
	#include <worldpos_vertex>
	#include <shadowmap_vertex>
	#include <fog_vertex>
}`,M1=`uniform vec3 color;
uniform float opacity;
#include <common>
#include <packing>
#include <fog_pars_fragment>
#include <bsdfs>
#include <lights_pars_begin>
#include <logdepthbuf_pars_fragment>
#include <shadowmap_pars_fragment>
#include <shadowmask_pars_fragment>
void main() {
	#include <logdepthbuf_fragment>
	gl_FragColor = vec4( color, opacity * ( 1.0 - getShadowMask() ) );
	#include <tonemapping_fragment>
	#include <colorspace_fragment>
	#include <fog_fragment>
}`,E1=`uniform float rotation;
uniform vec2 center;
#include <common>
#include <uv_pars_vertex>
#include <fog_pars_vertex>
#include <logdepthbuf_pars_vertex>
#include <clipping_planes_pars_vertex>
void main() {
	#include <uv_vertex>
	vec4 mvPosition = modelViewMatrix * vec4( 0.0, 0.0, 0.0, 1.0 );
	vec2 scale;
	scale.x = length( vec3( modelMatrix[ 0 ].x, modelMatrix[ 0 ].y, modelMatrix[ 0 ].z ) );
	scale.y = length( vec3( modelMatrix[ 1 ].x, modelMatrix[ 1 ].y, modelMatrix[ 1 ].z ) );
	#ifndef USE_SIZEATTENUATION
		bool isPerspective = isPerspectiveMatrix( projectionMatrix );
		if ( isPerspective ) scale *= - mvPosition.z;
	#endif
	vec2 alignedPosition = ( position.xy - ( center - vec2( 0.5 ) ) ) * scale;
	vec2 rotatedPosition;
	rotatedPosition.x = cos( rotation ) * alignedPosition.x - sin( rotation ) * alignedPosition.y;
	rotatedPosition.y = sin( rotation ) * alignedPosition.x + cos( rotation ) * alignedPosition.y;
	mvPosition.xy += rotatedPosition;
	gl_Position = projectionMatrix * mvPosition;
	#include <logdepthbuf_vertex>
	#include <clipping_planes_vertex>
	#include <fog_vertex>
}`,w1=`uniform vec3 diffuse;
uniform float opacity;
#include <common>
#include <uv_pars_fragment>
#include <map_pars_fragment>
#include <alphamap_pars_fragment>
#include <alphatest_pars_fragment>
#include <alphahash_pars_fragment>
#include <fog_pars_fragment>
#include <logdepthbuf_pars_fragment>
#include <clipping_planes_pars_fragment>
void main() {
	vec4 diffuseColor = vec4( diffuse, opacity );
	#include <clipping_planes_fragment>
	vec3 outgoingLight = vec3( 0.0 );
	#include <logdepthbuf_fragment>
	#include <map_fragment>
	#include <alphamap_fragment>
	#include <alphatest_fragment>
	#include <alphahash_fragment>
	outgoingLight = diffuseColor.rgb;
	#include <opaque_fragment>
	#include <tonemapping_fragment>
	#include <colorspace_fragment>
	#include <fog_fragment>
}`,dt={alphahash_fragment:jS,alphahash_pars_fragment:qS,alphamap_fragment:YS,alphamap_pars_fragment:KS,alphatest_fragment:$S,alphatest_pars_fragment:ZS,aomap_fragment:JS,aomap_pars_fragment:QS,batching_pars_vertex:eM,batching_vertex:tM,begin_vertex:nM,beginnormal_vertex:iM,bsdfs:rM,iridescence_fragment:sM,bumpmap_pars_fragment:oM,clipping_planes_fragment:aM,clipping_planes_pars_fragment:lM,clipping_planes_pars_vertex:uM,clipping_planes_vertex:cM,color_fragment:fM,color_pars_fragment:hM,color_pars_vertex:dM,color_vertex:pM,common:mM,cube_uv_reflection_fragment:gM,defaultnormal_vertex:_M,displacementmap_pars_vertex:vM,displacementmap_vertex:yM,emissivemap_fragment:xM,emissivemap_pars_fragment:SM,colorspace_fragment:MM,colorspace_pars_fragment:EM,envmap_fragment:wM,envmap_common_pars_fragment:TM,envmap_pars_fragment:AM,envmap_pars_vertex:CM,envmap_physical_pars_fragment:BM,envmap_vertex:RM,fog_vertex:PM,fog_pars_vertex:bM,fog_fragment:LM,fog_pars_fragment:DM,gradientmap_pars_fragment:NM,lightmap_pars_fragment:IM,lights_lambert_fragment:UM,lights_lambert_pars_fragment:FM,lights_pars_begin:OM,lights_toon_fragment:zM,lights_toon_pars_fragment:kM,lights_phong_fragment:HM,lights_phong_pars_fragment:VM,lights_physical_fragment:GM,lights_physical_pars_fragment:WM,lights_fragment_begin:XM,lights_fragment_maps:jM,lights_fragment_end:qM,logdepthbuf_fragment:YM,logdepthbuf_pars_fragment:KM,logdepthbuf_pars_vertex:$M,logdepthbuf_vertex:ZM,map_fragment:JM,map_pars_fragment:QM,map_particle_fragment:eE,map_particle_pars_fragment:tE,metalnessmap_fragment:nE,metalnessmap_pars_fragment:iE,morphinstance_vertex:rE,morphcolor_vertex:sE,morphnormal_vertex:oE,morphtarget_pars_vertex:aE,morphtarget_vertex:lE,normal_fragment_begin:uE,normal_fragment_maps:cE,normal_pars_fragment:fE,normal_pars_vertex:hE,normal_vertex:dE,normalmap_pars_fragment:pE,clearcoat_normal_fragment_begin:mE,clearcoat_normal_fragment_maps:gE,clearcoat_pars_fragment:_E,iridescence_pars_fragment:vE,opaque_fragment:yE,packing:xE,premultiplied_alpha_fragment:SE,project_vertex:ME,dithering_fragment:EE,dithering_pars_fragment:wE,roughnessmap_fragment:TE,roughnessmap_pars_fragment:AE,shadowmap_pars_fragment:CE,shadowmap_pars_vertex:RE,shadowmap_vertex:PE,shadowmask_pars_fragment:bE,skinbase_vertex:LE,skinning_pars_vertex:DE,skinning_vertex:NE,skinnormal_vertex:IE,specularmap_fragment:UE,specularmap_pars_fragment:FE,tonemapping_fragment:OE,tonemapping_pars_fragment:BE,transmission_fragment:zE,transmission_pars_fragment:kE,uv_pars_fragment:HE,uv_pars_vertex:VE,uv_vertex:GE,worldpos_vertex:WE,background_vert:XE,background_frag:jE,backgroundCube_vert:qE,backgroundCube_frag:YE,cube_vert:KE,cube_frag:$E,depth_vert:ZE,depth_frag:JE,distanceRGBA_vert:QE,distanceRGBA_frag:e1,equirect_vert:t1,equirect_frag:n1,linedashed_vert:i1,linedashed_frag:r1,meshbasic_vert:s1,meshbasic_frag:o1,meshlambert_vert:a1,meshlambert_frag:l1,meshmatcap_vert:u1,meshmatcap_frag:c1,meshnormal_vert:f1,meshnormal_frag:h1,meshphong_vert:d1,meshphong_frag:p1,meshphysical_vert:m1,meshphysical_frag:g1,meshtoon_vert:_1,meshtoon_frag:v1,points_vert:y1,points_frag:x1,shadow_vert:S1,shadow_frag:M1,sprite_vert:E1,sprite_frag:w1},ze={common:{diffuse:{value:new yt(16777215)},opacity:{value:1},map:{value:null},mapTransform:{value:new pt},alphaMap:{value:null},alphaMapTransform:{value:new pt},alphaTest:{value:0}},specularmap:{specularMap:{value:null},specularMapTransform:{value:new pt}},envmap:{envMap:{value:null},envMapRotation:{value:new pt},flipEnvMap:{value:-1},reflectivity:{value:1},ior:{value:1.5},refractionRatio:{value:.98}},aomap:{aoMap:{value:null},aoMapIntensity:{value:1},aoMapTransform:{value:new pt}},lightmap:{lightMap:{value:null},lightMapIntensity:{value:1},lightMapTransform:{value:new pt}},bumpmap:{bumpMap:{value:null},bumpMapTransform:{value:new pt},bumpScale:{value:1}},normalmap:{normalMap:{value:null},normalMapTransform:{value:new pt},normalScale:{value:new Pe(1,1)}},displacementmap:{displacementMap:{value:null},displacementMapTransform:{value:new pt},displacementScale:{value:1},displacementBias:{value:0}},emissivemap:{emissiveMap:{value:null},emissiveMapTransform:{value:new pt}},metalnessmap:{metalnessMap:{value:null},metalnessMapTransform:{value:new pt}},roughnessmap:{roughnessMap:{value:null},roughnessMapTransform:{value:new pt}},gradientmap:{gradientMap:{value:null}},fog:{fogDensity:{value:25e-5},fogNear:{value:1},fogFar:{value:2e3},fogColor:{value:new yt(16777215)}},lights:{ambientLightColor:{value:[]},lightProbe:{value:[]},directionalLights:{value:[],properties:{direction:{},color:{}}},directionalLightShadows:{value:[],properties:{shadowIntensity:1,shadowBias:{},shadowNormalBias:{},shadowRadius:{},shadowMapSize:{}}},directionalShadowMap:{value:[]},directionalShadowMatrix:{value:[]},spotLights:{value:[],properties:{color:{},position:{},direction:{},distance:{},coneCos:{},penumbraCos:{},decay:{}}},spotLightShadows:{value:[],properties:{shadowIntensity:1,shadowBias:{},shadowNormalBias:{},shadowRadius:{},shadowMapSize:{}}},spotLightMap:{value:[]},spotShadowMap:{value:[]},spotLightMatrix:{value:[]},pointLights:{value:[],properties:{color:{},position:{},decay:{},distance:{}}},pointLightShadows:{value:[],properties:{shadowIntensity:1,shadowBias:{},shadowNormalBias:{},shadowRadius:{},shadowMapSize:{},shadowCameraNear:{},shadowCameraFar:{}}},pointShadowMap:{value:[]},pointShadowMatrix:{value:[]},hemisphereLights:{value:[],properties:{direction:{},skyColor:{},groundColor:{}}},rectAreaLights:{value:[],properties:{color:{},position:{},width:{},height:{}}},ltc_1:{value:null},ltc_2:{value:null}},points:{diffuse:{value:new yt(16777215)},opacity:{value:1},size:{value:1},scale:{value:1},map:{value:null},alphaMap:{value:null},alphaMapTransform:{value:new pt},alphaTest:{value:0},uvTransform:{value:new pt}},sprite:{diffuse:{value:new yt(16777215)},opacity:{value:1},center:{value:new Pe(.5,.5)},rotation:{value:0},map:{value:null},mapTransform:{value:new pt},alphaMap:{value:null},alphaMapTransform:{value:new pt},alphaTest:{value:0}}},Ei={basic:{uniforms:An([ze.common,ze.specularmap,ze.envmap,ze.aomap,ze.lightmap,ze.fog]),vertexShader:dt.meshbasic_vert,fragmentShader:dt.meshbasic_frag},lambert:{uniforms:An([ze.common,ze.specularmap,ze.envmap,ze.aomap,ze.lightmap,ze.emissivemap,ze.bumpmap,ze.normalmap,ze.displacementmap,ze.fog,ze.lights,{emissive:{value:new yt(0)}}]),vertexShader:dt.meshlambert_vert,fragmentShader:dt.meshlambert_frag},phong:{uniforms:An([ze.common,ze.specularmap,ze.envmap,ze.aomap,ze.lightmap,ze.emissivemap,ze.bumpmap,ze.normalmap,ze.displacementmap,ze.fog,ze.lights,{emissive:{value:new yt(0)},specular:{value:new yt(1118481)},shininess:{value:30}}]),vertexShader:dt.meshphong_vert,fragmentShader:dt.meshphong_frag},standard:{uniforms:An([ze.common,ze.envmap,ze.aomap,ze.lightmap,ze.emissivemap,ze.bumpmap,ze.normalmap,ze.displacementmap,ze.roughnessmap,ze.metalnessmap,ze.fog,ze.lights,{emissive:{value:new yt(0)},roughness:{value:1},metalness:{value:0},envMapIntensity:{value:1}}]),vertexShader:dt.meshphysical_vert,fragmentShader:dt.meshphysical_frag},toon:{uniforms:An([ze.common,ze.aomap,ze.lightmap,ze.emissivemap,ze.bumpmap,ze.normalmap,ze.displacementmap,ze.gradientmap,ze.fog,ze.lights,{emissive:{value:new yt(0)}}]),vertexShader:dt.meshtoon_vert,fragmentShader:dt.meshtoon_frag},matcap:{uniforms:An([ze.common,ze.bumpmap,ze.normalmap,ze.displacementmap,ze.fog,{matcap:{value:null}}]),vertexShader:dt.meshmatcap_vert,fragmentShader:dt.meshmatcap_frag},points:{uniforms:An([ze.points,ze.fog]),vertexShader:dt.points_vert,fragmentShader:dt.points_frag},dashed:{uniforms:An([ze.common,ze.fog,{scale:{value:1},dashSize:{value:1},totalSize:{value:2}}]),vertexShader:dt.linedashed_vert,fragmentShader:dt.linedashed_frag},depth:{uniforms:An([ze.common,ze.displacementmap]),vertexShader:dt.depth_vert,fragmentShader:dt.depth_frag},normal:{uniforms:An([ze.common,ze.bumpmap,ze.normalmap,ze.displacementmap,{opacity:{value:1}}]),vertexShader:dt.meshnormal_vert,fragmentShader:dt.meshnormal_frag},sprite:{uniforms:An([ze.sprite,ze.fog]),vertexShader:dt.sprite_vert,fragmentShader:dt.sprite_frag},background:{uniforms:{uvTransform:{value:new pt},t2D:{value:null},backgroundIntensity:{value:1}},vertexShader:dt.background_vert,fragmentShader:dt.background_frag},backgroundCube:{uniforms:{envMap:{value:null},flipEnvMap:{value:-1},backgroundBlurriness:{value:0},backgroundIntensity:{value:1},backgroundRotation:{value:new pt}},vertexShader:dt.backgroundCube_vert,fragmentShader:dt.backgroundCube_frag},cube:{uniforms:{tCube:{value:null},tFlip:{value:-1},opacity:{value:1}},vertexShader:dt.cube_vert,fragmentShader:dt.cube_frag},equirect:{uniforms:{tEquirect:{value:null}},vertexShader:dt.equirect_vert,fragmentShader:dt.equirect_frag},distanceRGBA:{uniforms:An([ze.common,ze.displacementmap,{referencePosition:{value:new W},nearDistance:{value:1},farDistance:{value:1e3}}]),vertexShader:dt.distanceRGBA_vert,fragmentShader:dt.distanceRGBA_frag},shadow:{uniforms:An([ze.lights,ze.fog,{color:{value:new yt(0)},opacity:{value:1}}]),vertexShader:dt.shadow_vert,fragmentShader:dt.shadow_frag}};Ei.physical={uniforms:An([Ei.standard.uniforms,{clearcoat:{value:0},clearcoatMap:{value:null},clearcoatMapTransform:{value:new pt},clearcoatNormalMap:{value:null},clearcoatNormalMapTransform:{value:new pt},clearcoatNormalScale:{value:new Pe(1,1)},clearcoatRoughness:{value:0},clearcoatRoughnessMap:{value:null},clearcoatRoughnessMapTransform:{value:new pt},dispersion:{value:0},iridescence:{value:0},iridescenceMap:{value:null},iridescenceMapTransform:{value:new pt},iridescenceIOR:{value:1.3},iridescenceThicknessMinimum:{value:100},iridescenceThicknessMaximum:{value:400},iridescenceThicknessMap:{value:null},iridescenceThicknessMapTransform:{value:new pt},sheen:{value:0},sheenColor:{value:new yt(0)},sheenColorMap:{value:null},sheenColorMapTransform:{value:new pt},sheenRoughness:{value:1},sheenRoughnessMap:{value:null},sheenRoughnessMapTransform:{value:new pt},transmission:{value:0},transmissionMap:{value:null},transmissionMapTransform:{value:new pt},transmissionSamplerSize:{value:new Pe},transmissionSamplerMap:{value:null},thickness:{value:0},thicknessMap:{value:null},thicknessMapTransform:{value:new pt},attenuationDistance:{value:0},attenuationColor:{value:new yt(0)},specularColor:{value:new yt(1,1,1)},specularColorMap:{value:null},specularColorMapTransform:{value:new pt},specularIntensity:{value:1},specularIntensityMap:{value:null},specularIntensityMapTransform:{value:new pt},anisotropyVector:{value:new Pe},anisotropyMap:{value:null},anisotropyMapTransform:{value:new pt}}]),vertexShader:dt.meshphysical_vert,fragmentShader:dt.meshphysical_frag};const tu={r:0,b:0,g:0},Qr=new Ai,T1=new kt;function A1(s,e,t,i,o,a,u){const c=new yt(0);let h=a===!0?0:1,d,m,_=null,g=0,S=null;function M(b){let T=b.isScene===!0?b.background:null;return T&&T.isTexture&&(T=(b.backgroundBlurriness>0?t:e).get(T)),T}function E(b){let T=!1;const P=M(b);P===null?x(c,h):P&&P.isColor&&(x(P,1),T=!0);const H=s.xr.getEnvironmentBlendMode();H==="additive"?i.buffers.color.setClear(0,0,0,1,u):H==="alpha-blend"&&i.buffers.color.setClear(0,0,0,0,u),(s.autoClear||T)&&(i.buffers.depth.setTest(!0),i.buffers.depth.setMask(!0),i.buffers.color.setMask(!0),s.clear(s.autoClearColor,s.autoClearDepth,s.autoClearStencil))}function y(b,T){const P=M(T);P&&(P.isCubeTexture||P.mapping===Du)?(m===void 0&&(m=new zt(new On(1,1,1),new Nr({name:"BackgroundCubeMaterial",uniforms:Eo(Ei.backgroundCube.uniforms),vertexShader:Ei.backgroundCube.vertexShader,fragmentShader:Ei.backgroundCube.fragmentShader,side:Bn,depthTest:!1,depthWrite:!1,fog:!1})),m.geometry.deleteAttribute("normal"),m.geometry.deleteAttribute("uv"),m.onBeforeRender=function(H,D,N){this.matrixWorld.copyPosition(N.matrixWorld)},Object.defineProperty(m.material,"envMap",{get:function(){return this.uniforms.envMap.value}}),o.update(m)),Qr.copy(T.backgroundRotation),Qr.x*=-1,Qr.y*=-1,Qr.z*=-1,P.isCubeTexture&&P.isRenderTargetTexture===!1&&(Qr.y*=-1,Qr.z*=-1),m.material.uniforms.envMap.value=P,m.material.uniforms.flipEnvMap.value=P.isCubeTexture&&P.isRenderTargetTexture===!1?-1:1,m.material.uniforms.backgroundBlurriness.value=T.backgroundBlurriness,m.material.uniforms.backgroundIntensity.value=T.backgroundIntensity,m.material.uniforms.backgroundRotation.value.setFromMatrix4(T1.makeRotationFromEuler(Qr)),m.material.toneMapped=Tt.getTransfer(P.colorSpace)!==Ft,(_!==P||g!==P.version||S!==s.toneMapping)&&(m.material.needsUpdate=!0,_=P,g=P.version,S=s.toneMapping),m.layers.enableAll(),b.unshift(m,m.geometry,m.material,0,0,null)):P&&P.isTexture&&(d===void 0&&(d=new zt(new Ra(2,2),new Nr({name:"BackgroundMaterial",uniforms:Eo(Ei.background.uniforms),vertexShader:Ei.background.vertexShader,fragmentShader:Ei.background.fragmentShader,side:Dr,depthTest:!1,depthWrite:!1,fog:!1})),d.geometry.deleteAttribute("normal"),Object.defineProperty(d.material,"map",{get:function(){return this.uniforms.t2D.value}}),o.update(d)),d.material.uniforms.t2D.value=P,d.material.uniforms.backgroundIntensity.value=T.backgroundIntensity,d.material.toneMapped=Tt.getTransfer(P.colorSpace)!==Ft,P.matrixAutoUpdate===!0&&P.updateMatrix(),d.material.uniforms.uvTransform.value.copy(P.matrix),(_!==P||g!==P.version||S!==s.toneMapping)&&(d.material.needsUpdate=!0,_=P,g=P.version,S=s.toneMapping),d.layers.enableAll(),b.unshift(d,d.geometry,d.material,0,0,null))}function x(b,T){b.getRGB(tu,L_(s)),i.buffers.color.setClear(tu.r,tu.g,tu.b,T,u)}return{getClearColor:function(){return c},setClearColor:function(b,T=1){c.set(b),h=T,x(c,h)},getClearAlpha:function(){return h},setClearAlpha:function(b){h=b,x(c,h)},render:E,addToRenderList:y}}function C1(s,e){const t=s.getParameter(s.MAX_VERTEX_ATTRIBS),i={},o=g(null);let a=o,u=!1;function c(w,U,X,k,Y){let ae=!1;const te=_(k,X,U);a!==te&&(a=te,d(a.object)),ae=S(w,k,X,Y),ae&&M(w,k,X,Y),Y!==null&&e.update(Y,s.ELEMENT_ARRAY_BUFFER),(ae||u)&&(u=!1,P(w,U,X,k),Y!==null&&s.bindBuffer(s.ELEMENT_ARRAY_BUFFER,e.get(Y).buffer))}function h(){return s.createVertexArray()}function d(w){return s.bindVertexArray(w)}function m(w){return s.deleteVertexArray(w)}function _(w,U,X){const k=X.wireframe===!0;let Y=i[w.id];Y===void 0&&(Y={},i[w.id]=Y);let ae=Y[U.id];ae===void 0&&(ae={},Y[U.id]=ae);let te=ae[k];return te===void 0&&(te=g(h()),ae[k]=te),te}function g(w){const U=[],X=[],k=[];for(let Y=0;Y<t;Y++)U[Y]=0,X[Y]=0,k[Y]=0;return{geometry:null,program:null,wireframe:!1,newAttributes:U,enabledAttributes:X,attributeDivisors:k,object:w,attributes:{},index:null}}function S(w,U,X,k){const Y=a.attributes,ae=U.attributes;let te=0;const ce=X.getAttributes();for(const G in ce)if(ce[G].location>=0){const re=Y[G];let O=ae[G];if(O===void 0&&(G==="instanceMatrix"&&w.instanceMatrix&&(O=w.instanceMatrix),G==="instanceColor"&&w.instanceColor&&(O=w.instanceColor)),re===void 0||re.attribute!==O||O&&re.data!==O.data)return!0;te++}return a.attributesNum!==te||a.index!==k}function M(w,U,X,k){const Y={},ae=U.attributes;let te=0;const ce=X.getAttributes();for(const G in ce)if(ce[G].location>=0){let re=ae[G];re===void 0&&(G==="instanceMatrix"&&w.instanceMatrix&&(re=w.instanceMatrix),G==="instanceColor"&&w.instanceColor&&(re=w.instanceColor));const O={};O.attribute=re,re&&re.data&&(O.data=re.data),Y[G]=O,te++}a.attributes=Y,a.attributesNum=te,a.index=k}function E(){const w=a.newAttributes;for(let U=0,X=w.length;U<X;U++)w[U]=0}function y(w){x(w,0)}function x(w,U){const X=a.newAttributes,k=a.enabledAttributes,Y=a.attributeDivisors;X[w]=1,k[w]===0&&(s.enableVertexAttribArray(w),k[w]=1),Y[w]!==U&&(s.vertexAttribDivisor(w,U),Y[w]=U)}function b(){const w=a.newAttributes,U=a.enabledAttributes;for(let X=0,k=U.length;X<k;X++)U[X]!==w[X]&&(s.disableVertexAttribArray(X),U[X]=0)}function T(w,U,X,k,Y,ae,te){te===!0?s.vertexAttribIPointer(w,U,X,Y,ae):s.vertexAttribPointer(w,U,X,k,Y,ae)}function P(w,U,X,k){E();const Y=k.attributes,ae=X.getAttributes(),te=U.defaultAttributeValues;for(const ce in ae){const G=ae[ce];if(G.location>=0){let he=Y[ce];if(he===void 0&&(ce==="instanceMatrix"&&w.instanceMatrix&&(he=w.instanceMatrix),ce==="instanceColor"&&w.instanceColor&&(he=w.instanceColor)),he!==void 0){const re=he.normalized,O=he.itemSize,ie=e.get(he);if(ie===void 0)continue;const Ae=ie.buffer,Z=ie.type,ne=ie.bytesPerElement,xe=Z===s.INT||Z===s.UNSIGNED_INT||he.gpuType===pd;if(he.isInterleavedBufferAttribute){const de=he.data,ee=de.stride,we=he.offset;if(de.isInstancedInterleavedBuffer){for(let Ue=0;Ue<G.locationSize;Ue++)x(G.location+Ue,de.meshPerAttribute);w.isInstancedMesh!==!0&&k._maxInstanceCount===void 0&&(k._maxInstanceCount=de.meshPerAttribute*de.count)}else for(let Ue=0;Ue<G.locationSize;Ue++)y(G.location+Ue);s.bindBuffer(s.ARRAY_BUFFER,Ae);for(let Ue=0;Ue<G.locationSize;Ue++)T(G.location+Ue,O/G.locationSize,Z,re,ee*ne,(we+O/G.locationSize*Ue)*ne,xe)}else{if(he.isInstancedBufferAttribute){for(let de=0;de<G.locationSize;de++)x(G.location+de,he.meshPerAttribute);w.isInstancedMesh!==!0&&k._maxInstanceCount===void 0&&(k._maxInstanceCount=he.meshPerAttribute*he.count)}else for(let de=0;de<G.locationSize;de++)y(G.location+de);s.bindBuffer(s.ARRAY_BUFFER,Ae);for(let de=0;de<G.locationSize;de++)T(G.location+de,O/G.locationSize,Z,re,O*ne,O/G.locationSize*de*ne,xe)}}else if(te!==void 0){const re=te[ce];if(re!==void 0)switch(re.length){case 2:s.vertexAttrib2fv(G.location,re);break;case 3:s.vertexAttrib3fv(G.location,re);break;case 4:s.vertexAttrib4fv(G.location,re);break;default:s.vertexAttrib1fv(G.location,re)}}}}b()}function H(){z();for(const w in i){const U=i[w];for(const X in U){const k=U[X];for(const Y in k)m(k[Y].object),delete k[Y];delete U[X]}delete i[w]}}function D(w){if(i[w.id]===void 0)return;const U=i[w.id];for(const X in U){const k=U[X];for(const Y in k)m(k[Y].object),delete k[Y];delete U[X]}delete i[w.id]}function N(w){for(const U in i){const X=i[U];if(X[w.id]===void 0)continue;const k=X[w.id];for(const Y in k)m(k[Y].object),delete k[Y];delete X[w.id]}}function z(){C(),u=!0,a!==o&&(a=o,d(a.object))}function C(){o.geometry=null,o.program=null,o.wireframe=!1}return{setup:c,reset:z,resetDefaultState:C,dispose:H,releaseStatesOfGeometry:D,releaseStatesOfProgram:N,initAttributes:E,enableAttribute:y,disableUnusedAttributes:b}}function R1(s,e,t){let i;function o(d){i=d}function a(d,m){s.drawArrays(i,d,m),t.update(m,i,1)}function u(d,m,_){_!==0&&(s.drawArraysInstanced(i,d,m,_),t.update(m,i,_))}function c(d,m,_){if(_===0)return;e.get("WEBGL_multi_draw").multiDrawArraysWEBGL(i,d,0,m,0,_);let S=0;for(let M=0;M<_;M++)S+=m[M];t.update(S,i,1)}function h(d,m,_,g){if(_===0)return;const S=e.get("WEBGL_multi_draw");if(S===null)for(let M=0;M<d.length;M++)u(d[M],m[M],g[M]);else{S.multiDrawArraysInstancedWEBGL(i,d,0,m,0,g,0,_);let M=0;for(let E=0;E<_;E++)M+=m[E];for(let E=0;E<g.length;E++)t.update(M,i,g[E])}}this.setMode=o,this.render=a,this.renderInstances=u,this.renderMultiDraw=c,this.renderMultiDrawInstances=h}function P1(s,e,t,i){let o;function a(){if(o!==void 0)return o;if(e.has("EXT_texture_filter_anisotropic")===!0){const D=e.get("EXT_texture_filter_anisotropic");o=s.getParameter(D.MAX_TEXTURE_MAX_ANISOTROPY_EXT)}else o=0;return o}function u(D){return!(D!==mi&&i.convert(D)!==s.getParameter(s.IMPLEMENTATION_COLOR_READ_FORMAT))}function c(D){const N=D===Aa&&(e.has("EXT_color_buffer_half_float")||e.has("EXT_color_buffer_float"));return!(D!==Ki&&i.convert(D)!==s.getParameter(s.IMPLEMENTATION_COLOR_READ_TYPE)&&D!==Xi&&!N)}function h(D){if(D==="highp"){if(s.getShaderPrecisionFormat(s.VERTEX_SHADER,s.HIGH_FLOAT).precision>0&&s.getShaderPrecisionFormat(s.FRAGMENT_SHADER,s.HIGH_FLOAT).precision>0)return"highp";D="mediump"}return D==="mediump"&&s.getShaderPrecisionFormat(s.VERTEX_SHADER,s.MEDIUM_FLOAT).precision>0&&s.getShaderPrecisionFormat(s.FRAGMENT_SHADER,s.MEDIUM_FLOAT).precision>0?"mediump":"lowp"}let d=t.precision!==void 0?t.precision:"highp";const m=h(d);m!==d&&(console.warn("THREE.WebGLRenderer:",d,"not supported, using",m,"instead."),d=m);const _=t.logarithmicDepthBuffer===!0,g=s.getParameter(s.MAX_TEXTURE_IMAGE_UNITS),S=s.getParameter(s.MAX_VERTEX_TEXTURE_IMAGE_UNITS),M=s.getParameter(s.MAX_TEXTURE_SIZE),E=s.getParameter(s.MAX_CUBE_MAP_TEXTURE_SIZE),y=s.getParameter(s.MAX_VERTEX_ATTRIBS),x=s.getParameter(s.MAX_VERTEX_UNIFORM_VECTORS),b=s.getParameter(s.MAX_VARYING_VECTORS),T=s.getParameter(s.MAX_FRAGMENT_UNIFORM_VECTORS),P=S>0,H=s.getParameter(s.MAX_SAMPLES);return{isWebGL2:!0,getMaxAnisotropy:a,getMaxPrecision:h,textureFormatReadable:u,textureTypeReadable:c,precision:d,logarithmicDepthBuffer:_,maxTextures:g,maxVertexTextures:S,maxTextureSize:M,maxCubemapSize:E,maxAttributes:y,maxVertexUniforms:x,maxVaryings:b,maxFragmentUniforms:T,vertexTextures:P,maxSamples:H}}function b1(s){const e=this;let t=null,i=0,o=!1,a=!1;const u=new Cr,c=new pt,h={value:null,needsUpdate:!1};this.uniform=h,this.numPlanes=0,this.numIntersection=0,this.init=function(_,g){const S=_.length!==0||g||i!==0||o;return o=g,i=_.length,S},this.beginShadows=function(){a=!0,m(null)},this.endShadows=function(){a=!1},this.setGlobalState=function(_,g){t=m(_,g,0)},this.setState=function(_,g,S){const M=_.clippingPlanes,E=_.clipIntersection,y=_.clipShadows,x=s.get(_);if(!o||M===null||M.length===0||a&&!y)a?m(null):d();else{const b=a?0:i,T=b*4;let P=x.clippingState||null;h.value=P,P=m(M,g,T,S);for(let H=0;H!==T;++H)P[H]=t[H];x.clippingState=P,this.numIntersection=E?this.numPlanes:0,this.numPlanes+=b}};function d(){h.value!==t&&(h.value=t,h.needsUpdate=i>0),e.numPlanes=i,e.numIntersection=0}function m(_,g,S,M){const E=_!==null?_.length:0;let y=null;if(E!==0){if(y=h.value,M!==!0||y===null){const x=S+E*4,b=g.matrixWorldInverse;c.getNormalMatrix(b),(y===null||y.length<x)&&(y=new Float32Array(x));for(let T=0,P=S;T!==E;++T,P+=4)u.copy(_[T]).applyMatrix4(b,c),u.normal.toArray(y,P),y[P+3]=u.constant}h.value=y,h.needsUpdate=!0}return e.numPlanes=E,e.numIntersection=0,y}}function L1(s){let e=new WeakMap;function t(u,c){return c===Rh?u.mapping=yo:c===Ph&&(u.mapping=xo),u}function i(u){if(u&&u.isTexture){const c=u.mapping;if(c===Rh||c===Ph)if(e.has(u)){const h=e.get(u).texture;return t(h,u.mapping)}else{const h=u.image;if(h&&h.height>0){const d=new VS(h.height);return d.fromEquirectangularTexture(s,u),e.set(u,d),u.addEventListener("dispose",o),t(d.texture,u.mapping)}else return null}}return u}function o(u){const c=u.target;c.removeEventListener("dispose",o);const h=e.get(c);h!==void 0&&(e.delete(c),h.dispose())}function a(){e=new WeakMap}return{get:i,dispose:a}}class U_ extends D_{constructor(e=-1,t=1,i=1,o=-1,a=.1,u=2e3){super(),this.isOrthographicCamera=!0,this.type="OrthographicCamera",this.zoom=1,this.view=null,this.left=e,this.right=t,this.top=i,this.bottom=o,this.near=a,this.far=u,this.updateProjectionMatrix()}copy(e,t){return super.copy(e,t),this.left=e.left,this.right=e.right,this.top=e.top,this.bottom=e.bottom,this.near=e.near,this.far=e.far,this.zoom=e.zoom,this.view=e.view===null?null:Object.assign({},e.view),this}setViewOffset(e,t,i,o,a,u){this.view===null&&(this.view={enabled:!0,fullWidth:1,fullHeight:1,offsetX:0,offsetY:0,width:1,height:1}),this.view.enabled=!0,this.view.fullWidth=e,this.view.fullHeight=t,this.view.offsetX=i,this.view.offsetY=o,this.view.width=a,this.view.height=u,this.updateProjectionMatrix()}clearViewOffset(){this.view!==null&&(this.view.enabled=!1),this.updateProjectionMatrix()}updateProjectionMatrix(){const e=(this.right-this.left)/(2*this.zoom),t=(this.top-this.bottom)/(2*this.zoom),i=(this.right+this.left)/2,o=(this.top+this.bottom)/2;let a=i-e,u=i+e,c=o+t,h=o-t;if(this.view!==null&&this.view.enabled){const d=(this.right-this.left)/this.view.fullWidth/this.zoom,m=(this.top-this.bottom)/this.view.fullHeight/this.zoom;a+=d*this.view.offsetX,u=a+d*this.view.width,c-=m*this.view.offsetY,h=c-m*this.view.height}this.projectionMatrix.makeOrthographic(a,u,c,h,this.near,this.far,this.coordinateSystem),this.projectionMatrixInverse.copy(this.projectionMatrix).invert()}toJSON(e){const t=super.toJSON(e);return t.object.zoom=this.zoom,t.object.left=this.left,t.object.right=this.right,t.object.top=this.top,t.object.bottom=this.bottom,t.object.near=this.near,t.object.far=this.far,this.view!==null&&(t.object.view=Object.assign({},this.view)),t}}const co=4,f0=[.125,.215,.35,.446,.526,.582],is=20,fh=new U_,h0=new yt;let hh=null,dh=0,ph=0,mh=!1;const ts=(1+Math.sqrt(5))/2,to=1/ts,d0=[new W(-ts,to,0),new W(ts,to,0),new W(-to,0,ts),new W(to,0,ts),new W(0,ts,-to),new W(0,ts,to),new W(-1,1,-1),new W(1,1,-1),new W(-1,1,1),new W(1,1,1)];class p0{constructor(e){this._renderer=e,this._pingPongRenderTarget=null,this._lodMax=0,this._cubeSize=0,this._lodPlanes=[],this._sizeLods=[],this._sigmas=[],this._blurMaterial=null,this._cubemapMaterial=null,this._equirectMaterial=null,this._compileMaterial(this._blurMaterial)}fromScene(e,t=0,i=.1,o=100){hh=this._renderer.getRenderTarget(),dh=this._renderer.getActiveCubeFace(),ph=this._renderer.getActiveMipmapLevel(),mh=this._renderer.xr.enabled,this._renderer.xr.enabled=!1,this._setSize(256);const a=this._allocateTargets();return a.depthBuffer=!0,this._sceneToCubeUV(e,i,o,a),t>0&&this._blur(a,0,0,t),this._applyPMREM(a),this._cleanup(a),a}fromEquirectangular(e,t=null){return this._fromTexture(e,t)}fromCubemap(e,t=null){return this._fromTexture(e,t)}compileCubemapShader(){this._cubemapMaterial===null&&(this._cubemapMaterial=_0(),this._compileMaterial(this._cubemapMaterial))}compileEquirectangularShader(){this._equirectMaterial===null&&(this._equirectMaterial=g0(),this._compileMaterial(this._equirectMaterial))}dispose(){this._dispose(),this._cubemapMaterial!==null&&this._cubemapMaterial.dispose(),this._equirectMaterial!==null&&this._equirectMaterial.dispose()}_setSize(e){this._lodMax=Math.floor(Math.log2(e)),this._cubeSize=Math.pow(2,this._lodMax)}_dispose(){this._blurMaterial!==null&&this._blurMaterial.dispose(),this._pingPongRenderTarget!==null&&this._pingPongRenderTarget.dispose();for(let e=0;e<this._lodPlanes.length;e++)this._lodPlanes[e].dispose()}_cleanup(e){this._renderer.setRenderTarget(hh,dh,ph),this._renderer.xr.enabled=mh,e.scissorTest=!1,nu(e,0,0,e.width,e.height)}_fromTexture(e,t){e.mapping===yo||e.mapping===xo?this._setSize(e.image.length===0?16:e.image[0].width||e.image[0].image.width):this._setSize(e.image.width/4),hh=this._renderer.getRenderTarget(),dh=this._renderer.getActiveCubeFace(),ph=this._renderer.getActiveMipmapLevel(),mh=this._renderer.xr.enabled,this._renderer.xr.enabled=!1;const i=t||this._allocateTargets();return this._textureToCubeUV(e,i),this._applyPMREM(i),this._cleanup(i),i}_allocateTargets(){const e=3*Math.max(this._cubeSize,112),t=4*this._cubeSize,i={magFilter:mn,minFilter:mn,generateMipmaps:!1,type:Aa,format:mi,colorSpace:Ir,depthBuffer:!1},o=m0(e,t,i);if(this._pingPongRenderTarget===null||this._pingPongRenderTarget.width!==e||this._pingPongRenderTarget.height!==t){this._pingPongRenderTarget!==null&&this._dispose(),this._pingPongRenderTarget=m0(e,t,i);const{_lodMax:a}=this;({sizeLods:this._sizeLods,lodPlanes:this._lodPlanes,sigmas:this._sigmas}=D1(a)),this._blurMaterial=N1(a,e,t)}return o}_compileMaterial(e){const t=new zt(this._lodPlanes[0],e);this._renderer.compile(t,fh)}_sceneToCubeUV(e,t,i,o){const c=new ni(90,1,t,i),h=[1,-1,1,1,1,1],d=[1,1,1,-1,-1,-1],m=this._renderer,_=m.autoClear,g=m.toneMapping;m.getClearColor(h0),m.toneMapping=Lr,m.autoClear=!1;const S=new R_({name:"PMREM.Background",side:Bn,depthWrite:!1,depthTest:!1}),M=new zt(new On,S);let E=!1;const y=e.background;y?y.isColor&&(S.color.copy(y),e.background=null,E=!0):(S.color.copy(h0),E=!0);for(let x=0;x<6;x++){const b=x%3;b===0?(c.up.set(0,h[x],0),c.lookAt(d[x],0,0)):b===1?(c.up.set(0,0,h[x]),c.lookAt(0,d[x],0)):(c.up.set(0,h[x],0),c.lookAt(0,0,d[x]));const T=this._cubeSize;nu(o,b*T,x>2?T:0,T,T),m.setRenderTarget(o),E&&m.render(M,c),m.render(e,c)}M.geometry.dispose(),M.material.dispose(),m.toneMapping=g,m.autoClear=_,e.background=y}_textureToCubeUV(e,t){const i=this._renderer,o=e.mapping===yo||e.mapping===xo;o?(this._cubemapMaterial===null&&(this._cubemapMaterial=_0()),this._cubemapMaterial.uniforms.flipEnvMap.value=e.isRenderTargetTexture===!1?-1:1):this._equirectMaterial===null&&(this._equirectMaterial=g0());const a=o?this._cubemapMaterial:this._equirectMaterial,u=new zt(this._lodPlanes[0],a),c=a.uniforms;c.envMap.value=e;const h=this._cubeSize;nu(t,0,0,3*h,2*h),i.setRenderTarget(t),i.render(u,fh)}_applyPMREM(e){const t=this._renderer,i=t.autoClear;t.autoClear=!1;const o=this._lodPlanes.length;for(let a=1;a<o;a++){const u=Math.sqrt(this._sigmas[a]*this._sigmas[a]-this._sigmas[a-1]*this._sigmas[a-1]),c=d0[(o-a-1)%d0.length];this._blur(e,a-1,a,u,c)}t.autoClear=i}_blur(e,t,i,o,a){const u=this._pingPongRenderTarget;this._halfBlur(e,u,t,i,o,"latitudinal",a),this._halfBlur(u,e,i,i,o,"longitudinal",a)}_halfBlur(e,t,i,o,a,u,c){const h=this._renderer,d=this._blurMaterial;u!=="latitudinal"&&u!=="longitudinal"&&console.error("blur direction must be either latitudinal or longitudinal!");const m=3,_=new zt(this._lodPlanes[o],d),g=d.uniforms,S=this._sizeLods[i]-1,M=isFinite(a)?Math.PI/(2*S):2*Math.PI/(2*is-1),E=a/M,y=isFinite(a)?1+Math.floor(m*E):is;y>is&&console.warn(`sigmaRadians, ${a}, is too large and will clip, as it requested ${y} samples when the maximum is set to ${is}`);const x=[];let b=0;for(let N=0;N<is;++N){const z=N/E,C=Math.exp(-z*z/2);x.push(C),N===0?b+=C:N<y&&(b+=2*C)}for(let N=0;N<x.length;N++)x[N]=x[N]/b;g.envMap.value=e.texture,g.samples.value=y,g.weights.value=x,g.latitudinal.value=u==="latitudinal",c&&(g.poleAxis.value=c);const{_lodMax:T}=this;g.dTheta.value=M,g.mipInt.value=T-i;const P=this._sizeLods[o],H=3*P*(o>T-co?o-T+co:0),D=4*(this._cubeSize-P);nu(t,H,D,3*P,2*P),h.setRenderTarget(t),h.render(_,fh)}}function D1(s){const e=[],t=[],i=[];let o=s;const a=s-co+1+f0.length;for(let u=0;u<a;u++){const c=Math.pow(2,o);t.push(c);let h=1/c;u>s-co?h=f0[u-s+co-1]:u===0&&(h=0),i.push(h);const d=1/(c-2),m=-d,_=1+d,g=[m,m,_,m,_,_,m,m,_,_,m,_],S=6,M=6,E=3,y=2,x=1,b=new Float32Array(E*M*S),T=new Float32Array(y*M*S),P=new Float32Array(x*M*S);for(let D=0;D<S;D++){const N=D%3*2/3-1,z=D>2?0:-1,C=[N,z,0,N+2/3,z,0,N+2/3,z+1,0,N,z,0,N+2/3,z+1,0,N,z+1,0];b.set(C,E*M*D),T.set(g,y*M*D);const w=[D,D,D,D,D,D];P.set(w,x*M*D)}const H=new un;H.setAttribute("position",new gi(b,E)),H.setAttribute("uv",new gi(T,y)),H.setAttribute("faceIndex",new gi(P,x)),e.push(H),o>co&&o--}return{lodPlanes:e,sizeLods:t,sigmas:i}}function m0(s,e,t){const i=new as(s,e,t);return i.texture.mapping=Du,i.texture.name="PMREM.cubeUv",i.scissorTest=!0,i}function nu(s,e,t,i,o){s.viewport.set(e,t,i,o),s.scissor.set(e,t,i,o)}function N1(s,e,t){const i=new Float32Array(is),o=new W(0,1,0);return new Nr({name:"SphericalGaussianBlur",defines:{n:is,CUBEUV_TEXEL_WIDTH:1/e,CUBEUV_TEXEL_HEIGHT:1/t,CUBEUV_MAX_MIP:`${s}.0`},uniforms:{envMap:{value:null},samples:{value:1},weights:{value:i},latitudinal:{value:!1},dTheta:{value:0},mipInt:{value:0},poleAxis:{value:o}},vertexShader:Ed(),fragmentShader:`

			precision mediump float;
			precision mediump int;

			varying vec3 vOutputDirection;

			uniform sampler2D envMap;
			uniform int samples;
			uniform float weights[ n ];
			uniform bool latitudinal;
			uniform float dTheta;
			uniform float mipInt;
			uniform vec3 poleAxis;

			#define ENVMAP_TYPE_CUBE_UV
			#include <cube_uv_reflection_fragment>

			vec3 getSample( float theta, vec3 axis ) {

				float cosTheta = cos( theta );
				// Rodrigues' axis-angle rotation
				vec3 sampleDirection = vOutputDirection * cosTheta
					+ cross( axis, vOutputDirection ) * sin( theta )
					+ axis * dot( axis, vOutputDirection ) * ( 1.0 - cosTheta );

				return bilinearCubeUV( envMap, sampleDirection, mipInt );

			}

			void main() {

				vec3 axis = latitudinal ? poleAxis : cross( poleAxis, vOutputDirection );

				if ( all( equal( axis, vec3( 0.0 ) ) ) ) {

					axis = vec3( vOutputDirection.z, 0.0, - vOutputDirection.x );

				}

				axis = normalize( axis );

				gl_FragColor = vec4( 0.0, 0.0, 0.0, 1.0 );
				gl_FragColor.rgb += weights[ 0 ] * getSample( 0.0, axis );

				for ( int i = 1; i < n; i++ ) {

					if ( i >= samples ) {

						break;

					}

					float theta = dTheta * float( i );
					gl_FragColor.rgb += weights[ i ] * getSample( -1.0 * theta, axis );
					gl_FragColor.rgb += weights[ i ] * getSample( theta, axis );

				}

			}
		`,blending:br,depthTest:!1,depthWrite:!1})}function g0(){return new Nr({name:"EquirectangularToCubeUV",uniforms:{envMap:{value:null}},vertexShader:Ed(),fragmentShader:`

			precision mediump float;
			precision mediump int;

			varying vec3 vOutputDirection;

			uniform sampler2D envMap;

			#include <common>

			void main() {

				vec3 outputDirection = normalize( vOutputDirection );
				vec2 uv = equirectUv( outputDirection );

				gl_FragColor = vec4( texture2D ( envMap, uv ).rgb, 1.0 );

			}
		`,blending:br,depthTest:!1,depthWrite:!1})}function _0(){return new Nr({name:"CubemapToCubeUV",uniforms:{envMap:{value:null},flipEnvMap:{value:-1}},vertexShader:Ed(),fragmentShader:`

			precision mediump float;
			precision mediump int;

			uniform float flipEnvMap;

			varying vec3 vOutputDirection;

			uniform samplerCube envMap;

			void main() {

				gl_FragColor = textureCube( envMap, vec3( flipEnvMap * vOutputDirection.x, vOutputDirection.yz ) );

			}
		`,blending:br,depthTest:!1,depthWrite:!1})}function Ed(){return`

		precision mediump float;
		precision mediump int;

		attribute float faceIndex;

		varying vec3 vOutputDirection;

		// RH coordinate system; PMREM face-indexing convention
		vec3 getDirection( vec2 uv, float face ) {

			uv = 2.0 * uv - 1.0;

			vec3 direction = vec3( uv, 1.0 );

			if ( face == 0.0 ) {

				direction = direction.zyx; // ( 1, v, u ) pos x

			} else if ( face == 1.0 ) {

				direction = direction.xzy;
				direction.xz *= -1.0; // ( -u, 1, -v ) pos y

			} else if ( face == 2.0 ) {

				direction.x *= -1.0; // ( -u, v, 1 ) pos z

			} else if ( face == 3.0 ) {

				direction = direction.zyx;
				direction.xz *= -1.0; // ( -1, v, -u ) neg x

			} else if ( face == 4.0 ) {

				direction = direction.xzy;
				direction.xy *= -1.0; // ( -u, -1, v ) neg y

			} else if ( face == 5.0 ) {

				direction.z *= -1.0; // ( u, v, -1 ) neg z

			}

			return direction;

		}

		void main() {

			vOutputDirection = getDirection( uv, faceIndex );
			gl_Position = vec4( position, 1.0 );

		}
	`}function I1(s){let e=new WeakMap,t=null;function i(c){if(c&&c.isTexture){const h=c.mapping,d=h===Rh||h===Ph,m=h===yo||h===xo;if(d||m){let _=e.get(c);const g=_!==void 0?_.texture.pmremVersion:0;if(c.isRenderTargetTexture&&c.pmremVersion!==g)return t===null&&(t=new p0(s)),_=d?t.fromEquirectangular(c,_):t.fromCubemap(c,_),_.texture.pmremVersion=c.pmremVersion,e.set(c,_),_.texture;if(_!==void 0)return _.texture;{const S=c.image;return d&&S&&S.height>0||m&&S&&o(S)?(t===null&&(t=new p0(s)),_=d?t.fromEquirectangular(c):t.fromCubemap(c),_.texture.pmremVersion=c.pmremVersion,e.set(c,_),c.addEventListener("dispose",a),_.texture):null}}}return c}function o(c){let h=0;const d=6;for(let m=0;m<d;m++)c[m]!==void 0&&h++;return h===d}function a(c){const h=c.target;h.removeEventListener("dispose",a);const d=e.get(h);d!==void 0&&(e.delete(h),d.dispose())}function u(){e=new WeakMap,t!==null&&(t.dispose(),t=null)}return{get:i,dispose:u}}function U1(s){const e={};function t(i){if(e[i]!==void 0)return e[i];let o;switch(i){case"WEBGL_depth_texture":o=s.getExtension("WEBGL_depth_texture")||s.getExtension("MOZ_WEBGL_depth_texture")||s.getExtension("WEBKIT_WEBGL_depth_texture");break;case"EXT_texture_filter_anisotropic":o=s.getExtension("EXT_texture_filter_anisotropic")||s.getExtension("MOZ_EXT_texture_filter_anisotropic")||s.getExtension("WEBKIT_EXT_texture_filter_anisotropic");break;case"WEBGL_compressed_texture_s3tc":o=s.getExtension("WEBGL_compressed_texture_s3tc")||s.getExtension("MOZ_WEBGL_compressed_texture_s3tc")||s.getExtension("WEBKIT_WEBGL_compressed_texture_s3tc");break;case"WEBGL_compressed_texture_pvrtc":o=s.getExtension("WEBGL_compressed_texture_pvrtc")||s.getExtension("WEBKIT_WEBGL_compressed_texture_pvrtc");break;default:o=s.getExtension(i)}return e[i]=o,o}return{has:function(i){return t(i)!==null},init:function(){t("EXT_color_buffer_float"),t("WEBGL_clip_cull_distance"),t("OES_texture_float_linear"),t("EXT_color_buffer_half_float"),t("WEBGL_multisampled_render_to_texture"),t("WEBGL_render_shared_exponent")},get:function(i){const o=t(i);return o===null&&go("THREE.WebGLRenderer: "+i+" extension not supported."),o}}}function F1(s,e,t,i){const o={},a=new WeakMap;function u(_){const g=_.target;g.index!==null&&e.remove(g.index);for(const M in g.attributes)e.remove(g.attributes[M]);for(const M in g.morphAttributes){const E=g.morphAttributes[M];for(let y=0,x=E.length;y<x;y++)e.remove(E[y])}g.removeEventListener("dispose",u),delete o[g.id];const S=a.get(g);S&&(e.remove(S),a.delete(g)),i.releaseStatesOfGeometry(g),g.isInstancedBufferGeometry===!0&&delete g._maxInstanceCount,t.memory.geometries--}function c(_,g){return o[g.id]===!0||(g.addEventListener("dispose",u),o[g.id]=!0,t.memory.geometries++),g}function h(_){const g=_.attributes;for(const M in g)e.update(g[M],s.ARRAY_BUFFER);const S=_.morphAttributes;for(const M in S){const E=S[M];for(let y=0,x=E.length;y<x;y++)e.update(E[y],s.ARRAY_BUFFER)}}function d(_){const g=[],S=_.index,M=_.attributes.position;let E=0;if(S!==null){const b=S.array;E=S.version;for(let T=0,P=b.length;T<P;T+=3){const H=b[T+0],D=b[T+1],N=b[T+2];g.push(H,D,D,N,N,H)}}else if(M!==void 0){const b=M.array;E=M.version;for(let T=0,P=b.length/3-1;T<P;T+=3){const H=T+0,D=T+1,N=T+2;g.push(H,D,D,N,N,H)}}else return;const y=new(E_(g)?b_:P_)(g,1);y.version=E;const x=a.get(_);x&&e.remove(x),a.set(_,y)}function m(_){const g=a.get(_);if(g){const S=_.index;S!==null&&g.version<S.version&&d(_)}else d(_);return a.get(_)}return{get:c,update:h,getWireframeAttribute:m}}function O1(s,e,t){let i;function o(g){i=g}let a,u;function c(g){a=g.type,u=g.bytesPerElement}function h(g,S){s.drawElements(i,S,a,g*u),t.update(S,i,1)}function d(g,S,M){M!==0&&(s.drawElementsInstanced(i,S,a,g*u,M),t.update(S,i,M))}function m(g,S,M){if(M===0)return;e.get("WEBGL_multi_draw").multiDrawElementsWEBGL(i,S,0,a,g,0,M);let y=0;for(let x=0;x<M;x++)y+=S[x];t.update(y,i,1)}function _(g,S,M,E){if(M===0)return;const y=e.get("WEBGL_multi_draw");if(y===null)for(let x=0;x<g.length;x++)d(g[x]/u,S[x],E[x]);else{y.multiDrawElementsInstancedWEBGL(i,S,0,a,g,0,E,0,M);let x=0;for(let b=0;b<M;b++)x+=S[b];for(let b=0;b<E.length;b++)t.update(x,i,E[b])}}this.setMode=o,this.setIndex=c,this.render=h,this.renderInstances=d,this.renderMultiDraw=m,this.renderMultiDrawInstances=_}function B1(s){const e={geometries:0,textures:0},t={frame:0,calls:0,triangles:0,points:0,lines:0};function i(a,u,c){switch(t.calls++,u){case s.TRIANGLES:t.triangles+=c*(a/3);break;case s.LINES:t.lines+=c*(a/2);break;case s.LINE_STRIP:t.lines+=c*(a-1);break;case s.LINE_LOOP:t.lines+=c*a;break;case s.POINTS:t.points+=c*a;break;default:console.error("THREE.WebGLInfo: Unknown draw mode:",u);break}}function o(){t.calls=0,t.triangles=0,t.points=0,t.lines=0}return{memory:e,render:t,programs:null,autoReset:!0,reset:o,update:i}}function z1(s,e,t){const i=new WeakMap,o=new Qt;function a(u,c,h){const d=u.morphTargetInfluences,m=c.morphAttributes.position||c.morphAttributes.normal||c.morphAttributes.color,_=m!==void 0?m.length:0;let g=i.get(c);if(g===void 0||g.count!==_){let w=function(){z.dispose(),i.delete(c),c.removeEventListener("dispose",w)};var S=w;g!==void 0&&g.texture.dispose();const M=c.morphAttributes.position!==void 0,E=c.morphAttributes.normal!==void 0,y=c.morphAttributes.color!==void 0,x=c.morphAttributes.position||[],b=c.morphAttributes.normal||[],T=c.morphAttributes.color||[];let P=0;M===!0&&(P=1),E===!0&&(P=2),y===!0&&(P=3);let H=c.attributes.position.count*P,D=1;H>e.maxTextureSize&&(D=Math.ceil(H/e.maxTextureSize),H=e.maxTextureSize);const N=new Float32Array(H*D*4*_),z=new T_(N,H,D,_);z.type=Xi,z.needsUpdate=!0;const C=P*4;for(let U=0;U<_;U++){const X=x[U],k=b[U],Y=T[U],ae=H*D*4*U;for(let te=0;te<X.count;te++){const ce=te*C;M===!0&&(o.fromBufferAttribute(X,te),N[ae+ce+0]=o.x,N[ae+ce+1]=o.y,N[ae+ce+2]=o.z,N[ae+ce+3]=0),E===!0&&(o.fromBufferAttribute(k,te),N[ae+ce+4]=o.x,N[ae+ce+5]=o.y,N[ae+ce+6]=o.z,N[ae+ce+7]=0),y===!0&&(o.fromBufferAttribute(Y,te),N[ae+ce+8]=o.x,N[ae+ce+9]=o.y,N[ae+ce+10]=o.z,N[ae+ce+11]=Y.itemSize===4?o.w:1)}}g={count:_,texture:z,size:new Pe(H,D)},i.set(c,g),c.addEventListener("dispose",w)}if(u.isInstancedMesh===!0&&u.morphTexture!==null)h.getUniforms().setValue(s,"morphTexture",u.morphTexture,t);else{let M=0;for(let y=0;y<d.length;y++)M+=d[y];const E=c.morphTargetsRelative?1:1-M;h.getUniforms().setValue(s,"morphTargetBaseInfluence",E),h.getUniforms().setValue(s,"morphTargetInfluences",d)}h.getUniforms().setValue(s,"morphTargetsTexture",g.texture,t),h.getUniforms().setValue(s,"morphTargetsTextureSize",g.size)}return{update:a}}function k1(s,e,t,i){let o=new WeakMap;function a(h){const d=i.render.frame,m=h.geometry,_=e.get(h,m);if(o.get(_)!==d&&(e.update(_),o.set(_,d)),h.isInstancedMesh&&(h.hasEventListener("dispose",c)===!1&&h.addEventListener("dispose",c),o.get(h)!==d&&(t.update(h.instanceMatrix,s.ARRAY_BUFFER),h.instanceColor!==null&&t.update(h.instanceColor,s.ARRAY_BUFFER),o.set(h,d))),h.isSkinnedMesh){const g=h.skeleton;o.get(g)!==d&&(g.update(),o.set(g,d))}return _}function u(){o=new WeakMap}function c(h){const d=h.target;d.removeEventListener("dispose",c),t.remove(d.instanceMatrix),d.instanceColor!==null&&t.remove(d.instanceColor)}return{update:a,dispose:u}}class F_ extends Rn{constructor(e,t,i,o,a,u,c,h,d,m=mo){if(m!==mo&&m!==Mo)throw new Error("DepthTexture format must be either THREE.DepthFormat or THREE.DepthStencilFormat");i===void 0&&m===mo&&(i=os),i===void 0&&m===Mo&&(i=So),super(null,o,a,u,c,h,m,i,d),this.isDepthTexture=!0,this.image={width:e,height:t},this.magFilter=c!==void 0?c:ri,this.minFilter=h!==void 0?h:ri,this.flipY=!1,this.generateMipmaps=!1,this.compareFunction=null}copy(e){return super.copy(e),this.compareFunction=e.compareFunction,this}toJSON(e){const t=super.toJSON(e);return this.compareFunction!==null&&(t.compareFunction=this.compareFunction),t}}const O_=new Rn,v0=new F_(1,1),B_=new T_,z_=new AS,k_=new N_,y0=[],x0=[],S0=new Float32Array(16),M0=new Float32Array(9),E0=new Float32Array(4);function wo(s,e,t){const i=s[0];if(i<=0||i>0)return s;const o=e*t;let a=y0[o];if(a===void 0&&(a=new Float32Array(o),y0[o]=a),e!==0){i.toArray(a,0);for(let u=1,c=0;u!==e;++u)c+=t,s[u].toArray(a,c)}return a}function en(s,e){if(s.length!==e.length)return!1;for(let t=0,i=s.length;t<i;t++)if(s[t]!==e[t])return!1;return!0}function tn(s,e){for(let t=0,i=e.length;t<i;t++)s[t]=e[t]}function Uu(s,e){let t=x0[e];t===void 0&&(t=new Int32Array(e),x0[e]=t);for(let i=0;i!==e;++i)t[i]=s.allocateTextureUnit();return t}function H1(s,e){const t=this.cache;t[0]!==e&&(s.uniform1f(this.addr,e),t[0]=e)}function V1(s,e){const t=this.cache;if(e.x!==void 0)(t[0]!==e.x||t[1]!==e.y)&&(s.uniform2f(this.addr,e.x,e.y),t[0]=e.x,t[1]=e.y);else{if(en(t,e))return;s.uniform2fv(this.addr,e),tn(t,e)}}function G1(s,e){const t=this.cache;if(e.x!==void 0)(t[0]!==e.x||t[1]!==e.y||t[2]!==e.z)&&(s.uniform3f(this.addr,e.x,e.y,e.z),t[0]=e.x,t[1]=e.y,t[2]=e.z);else if(e.r!==void 0)(t[0]!==e.r||t[1]!==e.g||t[2]!==e.b)&&(s.uniform3f(this.addr,e.r,e.g,e.b),t[0]=e.r,t[1]=e.g,t[2]=e.b);else{if(en(t,e))return;s.uniform3fv(this.addr,e),tn(t,e)}}function W1(s,e){const t=this.cache;if(e.x!==void 0)(t[0]!==e.x||t[1]!==e.y||t[2]!==e.z||t[3]!==e.w)&&(s.uniform4f(this.addr,e.x,e.y,e.z,e.w),t[0]=e.x,t[1]=e.y,t[2]=e.z,t[3]=e.w);else{if(en(t,e))return;s.uniform4fv(this.addr,e),tn(t,e)}}function X1(s,e){const t=this.cache,i=e.elements;if(i===void 0){if(en(t,e))return;s.uniformMatrix2fv(this.addr,!1,e),tn(t,e)}else{if(en(t,i))return;E0.set(i),s.uniformMatrix2fv(this.addr,!1,E0),tn(t,i)}}function j1(s,e){const t=this.cache,i=e.elements;if(i===void 0){if(en(t,e))return;s.uniformMatrix3fv(this.addr,!1,e),tn(t,e)}else{if(en(t,i))return;M0.set(i),s.uniformMatrix3fv(this.addr,!1,M0),tn(t,i)}}function q1(s,e){const t=this.cache,i=e.elements;if(i===void 0){if(en(t,e))return;s.uniformMatrix4fv(this.addr,!1,e),tn(t,e)}else{if(en(t,i))return;S0.set(i),s.uniformMatrix4fv(this.addr,!1,S0),tn(t,i)}}function Y1(s,e){const t=this.cache;t[0]!==e&&(s.uniform1i(this.addr,e),t[0]=e)}function K1(s,e){const t=this.cache;if(e.x!==void 0)(t[0]!==e.x||t[1]!==e.y)&&(s.uniform2i(this.addr,e.x,e.y),t[0]=e.x,t[1]=e.y);else{if(en(t,e))return;s.uniform2iv(this.addr,e),tn(t,e)}}function $1(s,e){const t=this.cache;if(e.x!==void 0)(t[0]!==e.x||t[1]!==e.y||t[2]!==e.z)&&(s.uniform3i(this.addr,e.x,e.y,e.z),t[0]=e.x,t[1]=e.y,t[2]=e.z);else{if(en(t,e))return;s.uniform3iv(this.addr,e),tn(t,e)}}function Z1(s,e){const t=this.cache;if(e.x!==void 0)(t[0]!==e.x||t[1]!==e.y||t[2]!==e.z||t[3]!==e.w)&&(s.uniform4i(this.addr,e.x,e.y,e.z,e.w),t[0]=e.x,t[1]=e.y,t[2]=e.z,t[3]=e.w);else{if(en(t,e))return;s.uniform4iv(this.addr,e),tn(t,e)}}function J1(s,e){const t=this.cache;t[0]!==e&&(s.uniform1ui(this.addr,e),t[0]=e)}function Q1(s,e){const t=this.cache;if(e.x!==void 0)(t[0]!==e.x||t[1]!==e.y)&&(s.uniform2ui(this.addr,e.x,e.y),t[0]=e.x,t[1]=e.y);else{if(en(t,e))return;s.uniform2uiv(this.addr,e),tn(t,e)}}function ew(s,e){const t=this.cache;if(e.x!==void 0)(t[0]!==e.x||t[1]!==e.y||t[2]!==e.z)&&(s.uniform3ui(this.addr,e.x,e.y,e.z),t[0]=e.x,t[1]=e.y,t[2]=e.z);else{if(en(t,e))return;s.uniform3uiv(this.addr,e),tn(t,e)}}function tw(s,e){const t=this.cache;if(e.x!==void 0)(t[0]!==e.x||t[1]!==e.y||t[2]!==e.z||t[3]!==e.w)&&(s.uniform4ui(this.addr,e.x,e.y,e.z,e.w),t[0]=e.x,t[1]=e.y,t[2]=e.z,t[3]=e.w);else{if(en(t,e))return;s.uniform4uiv(this.addr,e),tn(t,e)}}function nw(s,e,t){const i=this.cache,o=t.allocateTextureUnit();i[0]!==o&&(s.uniform1i(this.addr,o),i[0]=o);let a;this.type===s.SAMPLER_2D_SHADOW?(v0.compareFunction=M_,a=v0):a=O_,t.setTexture2D(e||a,o)}function iw(s,e,t){const i=this.cache,o=t.allocateTextureUnit();i[0]!==o&&(s.uniform1i(this.addr,o),i[0]=o),t.setTexture3D(e||z_,o)}function rw(s,e,t){const i=this.cache,o=t.allocateTextureUnit();i[0]!==o&&(s.uniform1i(this.addr,o),i[0]=o),t.setTextureCube(e||k_,o)}function sw(s,e,t){const i=this.cache,o=t.allocateTextureUnit();i[0]!==o&&(s.uniform1i(this.addr,o),i[0]=o),t.setTexture2DArray(e||B_,o)}function ow(s){switch(s){case 5126:return H1;case 35664:return V1;case 35665:return G1;case 35666:return W1;case 35674:return X1;case 35675:return j1;case 35676:return q1;case 5124:case 35670:return Y1;case 35667:case 35671:return K1;case 35668:case 35672:return $1;case 35669:case 35673:return Z1;case 5125:return J1;case 36294:return Q1;case 36295:return ew;case 36296:return tw;case 35678:case 36198:case 36298:case 36306:case 35682:return nw;case 35679:case 36299:case 36307:return iw;case 35680:case 36300:case 36308:case 36293:return rw;case 36289:case 36303:case 36311:case 36292:return sw}}function aw(s,e){s.uniform1fv(this.addr,e)}function lw(s,e){const t=wo(e,this.size,2);s.uniform2fv(this.addr,t)}function uw(s,e){const t=wo(e,this.size,3);s.uniform3fv(this.addr,t)}function cw(s,e){const t=wo(e,this.size,4);s.uniform4fv(this.addr,t)}function fw(s,e){const t=wo(e,this.size,4);s.uniformMatrix2fv(this.addr,!1,t)}function hw(s,e){const t=wo(e,this.size,9);s.uniformMatrix3fv(this.addr,!1,t)}function dw(s,e){const t=wo(e,this.size,16);s.uniformMatrix4fv(this.addr,!1,t)}function pw(s,e){s.uniform1iv(this.addr,e)}function mw(s,e){s.uniform2iv(this.addr,e)}function gw(s,e){s.uniform3iv(this.addr,e)}function _w(s,e){s.uniform4iv(this.addr,e)}function vw(s,e){s.uniform1uiv(this.addr,e)}function yw(s,e){s.uniform2uiv(this.addr,e)}function xw(s,e){s.uniform3uiv(this.addr,e)}function Sw(s,e){s.uniform4uiv(this.addr,e)}function Mw(s,e,t){const i=this.cache,o=e.length,a=Uu(t,o);en(i,a)||(s.uniform1iv(this.addr,a),tn(i,a));for(let u=0;u!==o;++u)t.setTexture2D(e[u]||O_,a[u])}function Ew(s,e,t){const i=this.cache,o=e.length,a=Uu(t,o);en(i,a)||(s.uniform1iv(this.addr,a),tn(i,a));for(let u=0;u!==o;++u)t.setTexture3D(e[u]||z_,a[u])}function ww(s,e,t){const i=this.cache,o=e.length,a=Uu(t,o);en(i,a)||(s.uniform1iv(this.addr,a),tn(i,a));for(let u=0;u!==o;++u)t.setTextureCube(e[u]||k_,a[u])}function Tw(s,e,t){const i=this.cache,o=e.length,a=Uu(t,o);en(i,a)||(s.uniform1iv(this.addr,a),tn(i,a));for(let u=0;u!==o;++u)t.setTexture2DArray(e[u]||B_,a[u])}function Aw(s){switch(s){case 5126:return aw;case 35664:return lw;case 35665:return uw;case 35666:return cw;case 35674:return fw;case 35675:return hw;case 35676:return dw;case 5124:case 35670:return pw;case 35667:case 35671:return mw;case 35668:case 35672:return gw;case 35669:case 35673:return _w;case 5125:return vw;case 36294:return yw;case 36295:return xw;case 36296:return Sw;case 35678:case 36198:case 36298:case 36306:case 35682:return Mw;case 35679:case 36299:case 36307:return Ew;case 35680:case 36300:case 36308:case 36293:return ww;case 36289:case 36303:case 36311:case 36292:return Tw}}class Cw{constructor(e,t,i){this.id=e,this.addr=i,this.cache=[],this.type=t.type,this.setValue=ow(t.type)}}class Rw{constructor(e,t,i){this.id=e,this.addr=i,this.cache=[],this.type=t.type,this.size=t.size,this.setValue=Aw(t.type)}}class Pw{constructor(e){this.id=e,this.seq=[],this.map={}}setValue(e,t,i){const o=this.seq;for(let a=0,u=o.length;a!==u;++a){const c=o[a];c.setValue(e,t[c.id],i)}}}const gh=/(\w+)(\])?(\[|\.)?/g;function w0(s,e){s.seq.push(e),s.map[e.id]=e}function bw(s,e,t){const i=s.name,o=i.length;for(gh.lastIndex=0;;){const a=gh.exec(i),u=gh.lastIndex;let c=a[1];const h=a[2]==="]",d=a[3];if(h&&(c=c|0),d===void 0||d==="["&&u+2===o){w0(t,d===void 0?new Cw(c,s,e):new Rw(c,s,e));break}else{let _=t.map[c];_===void 0&&(_=new Pw(c),w0(t,_)),t=_}}}class Mu{constructor(e,t){this.seq=[],this.map={};const i=e.getProgramParameter(t,e.ACTIVE_UNIFORMS);for(let o=0;o<i;++o){const a=e.getActiveUniform(t,o),u=e.getUniformLocation(t,a.name);bw(a,u,this)}}setValue(e,t,i,o){const a=this.map[t];a!==void 0&&a.setValue(e,i,o)}setOptional(e,t,i){const o=t[i];o!==void 0&&this.setValue(e,i,o)}static upload(e,t,i,o){for(let a=0,u=t.length;a!==u;++a){const c=t[a],h=i[c.id];h.needsUpdate!==!1&&c.setValue(e,h.value,o)}}static seqWithValue(e,t){const i=[];for(let o=0,a=e.length;o!==a;++o){const u=e[o];u.id in t&&i.push(u)}return i}}function T0(s,e,t){const i=s.createShader(e);return s.shaderSource(i,t),s.compileShader(i),i}const Lw=37297;let Dw=0;function Nw(s,e){const t=s.split(`
`),i=[],o=Math.max(e-6,0),a=Math.min(e+6,t.length);for(let u=o;u<a;u++){const c=u+1;i.push(`${c===e?">":" "} ${c}: ${t[u]}`)}return i.join(`
`)}function Iw(s){const e=Tt.getPrimaries(Tt.workingColorSpace),t=Tt.getPrimaries(s);let i;switch(e===t?i="":e===Au&&t===Tu?i="LinearDisplayP3ToLinearSRGB":e===Tu&&t===Au&&(i="LinearSRGBToLinearDisplayP3"),s){case Ir:case Nu:return[i,"LinearTransferOETF"];case Mi:case xd:return[i,"sRGBTransferOETF"];default:return console.warn("THREE.WebGLProgram: Unsupported color space:",s),[i,"LinearTransferOETF"]}}function A0(s,e,t){const i=s.getShaderParameter(e,s.COMPILE_STATUS),o=s.getShaderInfoLog(e).trim();if(i&&o==="")return"";const a=/ERROR: 0:(\d+)/.exec(o);if(a){const u=parseInt(a[1]);return t.toUpperCase()+`

`+o+`

`+Nw(s.getShaderSource(e),u)}else return o}function Uw(s,e){const t=Iw(e);return`vec4 ${s}( vec4 value ) { return ${t[0]}( ${t[1]}( value ) ); }`}function Fw(s,e){let t;switch(e){case Qx:t="Linear";break;case eS:t="Reinhard";break;case tS:t="Cineon";break;case nS:t="ACESFilmic";break;case rS:t="AgX";break;case sS:t="Neutral";break;case iS:t="Custom";break;default:console.warn("THREE.WebGLProgram: Unsupported toneMapping:",e),t="Linear"}return"vec3 "+s+"( vec3 color ) { return "+t+"ToneMapping( color ); }"}const iu=new W;function Ow(){Tt.getLuminanceCoefficients(iu);const s=iu.x.toFixed(4),e=iu.y.toFixed(4),t=iu.z.toFixed(4);return["float luminance( const in vec3 rgb ) {",`	const vec3 weights = vec3( ${s}, ${e}, ${t} );`,"	return dot( weights, rgb );","}"].join(`
`)}function Bw(s){return[s.extensionClipCullDistance?"#extension GL_ANGLE_clip_cull_distance : require":"",s.extensionMultiDraw?"#extension GL_ANGLE_multi_draw : require":""].filter(ga).join(`
`)}function zw(s){const e=[];for(const t in s){const i=s[t];i!==!1&&e.push("#define "+t+" "+i)}return e.join(`
`)}function kw(s,e){const t={},i=s.getProgramParameter(e,s.ACTIVE_ATTRIBUTES);for(let o=0;o<i;o++){const a=s.getActiveAttrib(e,o),u=a.name;let c=1;a.type===s.FLOAT_MAT2&&(c=2),a.type===s.FLOAT_MAT3&&(c=3),a.type===s.FLOAT_MAT4&&(c=4),t[u]={type:a.type,location:s.getAttribLocation(e,u),locationSize:c}}return t}function ga(s){return s!==""}function C0(s,e){const t=e.numSpotLightShadows+e.numSpotLightMaps-e.numSpotLightShadowsWithMaps;return s.replace(/NUM_DIR_LIGHTS/g,e.numDirLights).replace(/NUM_SPOT_LIGHTS/g,e.numSpotLights).replace(/NUM_SPOT_LIGHT_MAPS/g,e.numSpotLightMaps).replace(/NUM_SPOT_LIGHT_COORDS/g,t).replace(/NUM_RECT_AREA_LIGHTS/g,e.numRectAreaLights).replace(/NUM_POINT_LIGHTS/g,e.numPointLights).replace(/NUM_HEMI_LIGHTS/g,e.numHemiLights).replace(/NUM_DIR_LIGHT_SHADOWS/g,e.numDirLightShadows).replace(/NUM_SPOT_LIGHT_SHADOWS_WITH_MAPS/g,e.numSpotLightShadowsWithMaps).replace(/NUM_SPOT_LIGHT_SHADOWS/g,e.numSpotLightShadows).replace(/NUM_POINT_LIGHT_SHADOWS/g,e.numPointLightShadows)}function R0(s,e){return s.replace(/NUM_CLIPPING_PLANES/g,e.numClippingPlanes).replace(/UNION_CLIPPING_PLANES/g,e.numClippingPlanes-e.numClipIntersection)}const Hw=/^[ \t]*#include +<([\w\d./]+)>/gm;function od(s){return s.replace(Hw,Gw)}const Vw=new Map;function Gw(s,e){let t=dt[e];if(t===void 0){const i=Vw.get(e);if(i!==void 0)t=dt[i],console.warn('THREE.WebGLRenderer: Shader chunk "%s" has been deprecated. Use "%s" instead.',e,i);else throw new Error("Can not resolve #include <"+e+">")}return od(t)}const Ww=/#pragma unroll_loop_start\s+for\s*\(\s*int\s+i\s*=\s*(\d+)\s*;\s*i\s*<\s*(\d+)\s*;\s*i\s*\+\+\s*\)\s*{([\s\S]+?)}\s+#pragma unroll_loop_end/g;function P0(s){return s.replace(Ww,Xw)}function Xw(s,e,t,i){let o="";for(let a=parseInt(e);a<parseInt(t);a++)o+=i.replace(/\[\s*i\s*\]/g,"[ "+a+" ]").replace(/UNROLLED_LOOP_INDEX/g,a);return o}function b0(s){let e=`precision ${s.precision} float;
	precision ${s.precision} int;
	precision ${s.precision} sampler2D;
	precision ${s.precision} samplerCube;
	precision ${s.precision} sampler3D;
	precision ${s.precision} sampler2DArray;
	precision ${s.precision} sampler2DShadow;
	precision ${s.precision} samplerCubeShadow;
	precision ${s.precision} sampler2DArrayShadow;
	precision ${s.precision} isampler2D;
	precision ${s.precision} isampler3D;
	precision ${s.precision} isamplerCube;
	precision ${s.precision} isampler2DArray;
	precision ${s.precision} usampler2D;
	precision ${s.precision} usampler3D;
	precision ${s.precision} usamplerCube;
	precision ${s.precision} usampler2DArray;
	`;return s.precision==="highp"?e+=`
#define HIGH_PRECISION`:s.precision==="mediump"?e+=`
#define MEDIUM_PRECISION`:s.precision==="lowp"&&(e+=`
#define LOW_PRECISION`),e}function jw(s){let e="SHADOWMAP_TYPE_BASIC";return s.shadowMapType===l_?e="SHADOWMAP_TYPE_PCF":s.shadowMapType===Tx?e="SHADOWMAP_TYPE_PCF_SOFT":s.shadowMapType===Wi&&(e="SHADOWMAP_TYPE_VSM"),e}function qw(s){let e="ENVMAP_TYPE_CUBE";if(s.envMap)switch(s.envMapMode){case yo:case xo:e="ENVMAP_TYPE_CUBE";break;case Du:e="ENVMAP_TYPE_CUBE_UV";break}return e}function Yw(s){let e="ENVMAP_MODE_REFLECTION";if(s.envMap)switch(s.envMapMode){case xo:e="ENVMAP_MODE_REFRACTION";break}return e}function Kw(s){let e="ENVMAP_BLENDING_NONE";if(s.envMap)switch(s.combine){case u_:e="ENVMAP_BLENDING_MULTIPLY";break;case Zx:e="ENVMAP_BLENDING_MIX";break;case Jx:e="ENVMAP_BLENDING_ADD";break}return e}function $w(s){const e=s.envMapCubeUVHeight;if(e===null)return null;const t=Math.log2(e)-2,i=1/e;return{texelWidth:1/(3*Math.max(Math.pow(2,t),112)),texelHeight:i,maxMip:t}}function Zw(s,e,t,i){const o=s.getContext(),a=t.defines;let u=t.vertexShader,c=t.fragmentShader;const h=jw(t),d=qw(t),m=Yw(t),_=Kw(t),g=$w(t),S=Bw(t),M=zw(a),E=o.createProgram();let y,x,b=t.glslVersion?"#version "+t.glslVersion+`
`:"";t.isRawShaderMaterial?(y=["#define SHADER_TYPE "+t.shaderType,"#define SHADER_NAME "+t.shaderName,M].filter(ga).join(`
`),y.length>0&&(y+=`
`),x=["#define SHADER_TYPE "+t.shaderType,"#define SHADER_NAME "+t.shaderName,M].filter(ga).join(`
`),x.length>0&&(x+=`
`)):(y=[b0(t),"#define SHADER_TYPE "+t.shaderType,"#define SHADER_NAME "+t.shaderName,M,t.extensionClipCullDistance?"#define USE_CLIP_DISTANCE":"",t.batching?"#define USE_BATCHING":"",t.batchingColor?"#define USE_BATCHING_COLOR":"",t.instancing?"#define USE_INSTANCING":"",t.instancingColor?"#define USE_INSTANCING_COLOR":"",t.instancingMorph?"#define USE_INSTANCING_MORPH":"",t.useFog&&t.fog?"#define USE_FOG":"",t.useFog&&t.fogExp2?"#define FOG_EXP2":"",t.map?"#define USE_MAP":"",t.envMap?"#define USE_ENVMAP":"",t.envMap?"#define "+m:"",t.lightMap?"#define USE_LIGHTMAP":"",t.aoMap?"#define USE_AOMAP":"",t.bumpMap?"#define USE_BUMPMAP":"",t.normalMap?"#define USE_NORMALMAP":"",t.normalMapObjectSpace?"#define USE_NORMALMAP_OBJECTSPACE":"",t.normalMapTangentSpace?"#define USE_NORMALMAP_TANGENTSPACE":"",t.displacementMap?"#define USE_DISPLACEMENTMAP":"",t.emissiveMap?"#define USE_EMISSIVEMAP":"",t.anisotropy?"#define USE_ANISOTROPY":"",t.anisotropyMap?"#define USE_ANISOTROPYMAP":"",t.clearcoatMap?"#define USE_CLEARCOATMAP":"",t.clearcoatRoughnessMap?"#define USE_CLEARCOAT_ROUGHNESSMAP":"",t.clearcoatNormalMap?"#define USE_CLEARCOAT_NORMALMAP":"",t.iridescenceMap?"#define USE_IRIDESCENCEMAP":"",t.iridescenceThicknessMap?"#define USE_IRIDESCENCE_THICKNESSMAP":"",t.specularMap?"#define USE_SPECULARMAP":"",t.specularColorMap?"#define USE_SPECULAR_COLORMAP":"",t.specularIntensityMap?"#define USE_SPECULAR_INTENSITYMAP":"",t.roughnessMap?"#define USE_ROUGHNESSMAP":"",t.metalnessMap?"#define USE_METALNESSMAP":"",t.alphaMap?"#define USE_ALPHAMAP":"",t.alphaHash?"#define USE_ALPHAHASH":"",t.transmission?"#define USE_TRANSMISSION":"",t.transmissionMap?"#define USE_TRANSMISSIONMAP":"",t.thicknessMap?"#define USE_THICKNESSMAP":"",t.sheenColorMap?"#define USE_SHEEN_COLORMAP":"",t.sheenRoughnessMap?"#define USE_SHEEN_ROUGHNESSMAP":"",t.mapUv?"#define MAP_UV "+t.mapUv:"",t.alphaMapUv?"#define ALPHAMAP_UV "+t.alphaMapUv:"",t.lightMapUv?"#define LIGHTMAP_UV "+t.lightMapUv:"",t.aoMapUv?"#define AOMAP_UV "+t.aoMapUv:"",t.emissiveMapUv?"#define EMISSIVEMAP_UV "+t.emissiveMapUv:"",t.bumpMapUv?"#define BUMPMAP_UV "+t.bumpMapUv:"",t.normalMapUv?"#define NORMALMAP_UV "+t.normalMapUv:"",t.displacementMapUv?"#define DISPLACEMENTMAP_UV "+t.displacementMapUv:"",t.metalnessMapUv?"#define METALNESSMAP_UV "+t.metalnessMapUv:"",t.roughnessMapUv?"#define ROUGHNESSMAP_UV "+t.roughnessMapUv:"",t.anisotropyMapUv?"#define ANISOTROPYMAP_UV "+t.anisotropyMapUv:"",t.clearcoatMapUv?"#define CLEARCOATMAP_UV "+t.clearcoatMapUv:"",t.clearcoatNormalMapUv?"#define CLEARCOAT_NORMALMAP_UV "+t.clearcoatNormalMapUv:"",t.clearcoatRoughnessMapUv?"#define CLEARCOAT_ROUGHNESSMAP_UV "+t.clearcoatRoughnessMapUv:"",t.iridescenceMapUv?"#define IRIDESCENCEMAP_UV "+t.iridescenceMapUv:"",t.iridescenceThicknessMapUv?"#define IRIDESCENCE_THICKNESSMAP_UV "+t.iridescenceThicknessMapUv:"",t.sheenColorMapUv?"#define SHEEN_COLORMAP_UV "+t.sheenColorMapUv:"",t.sheenRoughnessMapUv?"#define SHEEN_ROUGHNESSMAP_UV "+t.sheenRoughnessMapUv:"",t.specularMapUv?"#define SPECULARMAP_UV "+t.specularMapUv:"",t.specularColorMapUv?"#define SPECULAR_COLORMAP_UV "+t.specularColorMapUv:"",t.specularIntensityMapUv?"#define SPECULAR_INTENSITYMAP_UV "+t.specularIntensityMapUv:"",t.transmissionMapUv?"#define TRANSMISSIONMAP_UV "+t.transmissionMapUv:"",t.thicknessMapUv?"#define THICKNESSMAP_UV "+t.thicknessMapUv:"",t.vertexTangents&&t.flatShading===!1?"#define USE_TANGENT":"",t.vertexColors?"#define USE_COLOR":"",t.vertexAlphas?"#define USE_COLOR_ALPHA":"",t.vertexUv1s?"#define USE_UV1":"",t.vertexUv2s?"#define USE_UV2":"",t.vertexUv3s?"#define USE_UV3":"",t.pointsUvs?"#define USE_POINTS_UV":"",t.flatShading?"#define FLAT_SHADED":"",t.skinning?"#define USE_SKINNING":"",t.morphTargets?"#define USE_MORPHTARGETS":"",t.morphNormals&&t.flatShading===!1?"#define USE_MORPHNORMALS":"",t.morphColors?"#define USE_MORPHCOLORS":"",t.morphTargetsCount>0?"#define MORPHTARGETS_TEXTURE_STRIDE "+t.morphTextureStride:"",t.morphTargetsCount>0?"#define MORPHTARGETS_COUNT "+t.morphTargetsCount:"",t.doubleSided?"#define DOUBLE_SIDED":"",t.flipSided?"#define FLIP_SIDED":"",t.shadowMapEnabled?"#define USE_SHADOWMAP":"",t.shadowMapEnabled?"#define "+h:"",t.sizeAttenuation?"#define USE_SIZEATTENUATION":"",t.numLightProbes>0?"#define USE_LIGHT_PROBES":"",t.logarithmicDepthBuffer?"#define USE_LOGDEPTHBUF":"","uniform mat4 modelMatrix;","uniform mat4 modelViewMatrix;","uniform mat4 projectionMatrix;","uniform mat4 viewMatrix;","uniform mat3 normalMatrix;","uniform vec3 cameraPosition;","uniform bool isOrthographic;","#ifdef USE_INSTANCING","	attribute mat4 instanceMatrix;","#endif","#ifdef USE_INSTANCING_COLOR","	attribute vec3 instanceColor;","#endif","#ifdef USE_INSTANCING_MORPH","	uniform sampler2D morphTexture;","#endif","attribute vec3 position;","attribute vec3 normal;","attribute vec2 uv;","#ifdef USE_UV1","	attribute vec2 uv1;","#endif","#ifdef USE_UV2","	attribute vec2 uv2;","#endif","#ifdef USE_UV3","	attribute vec2 uv3;","#endif","#ifdef USE_TANGENT","	attribute vec4 tangent;","#endif","#if defined( USE_COLOR_ALPHA )","	attribute vec4 color;","#elif defined( USE_COLOR )","	attribute vec3 color;","#endif","#ifdef USE_SKINNING","	attribute vec4 skinIndex;","	attribute vec4 skinWeight;","#endif",`
`].filter(ga).join(`
`),x=[b0(t),"#define SHADER_TYPE "+t.shaderType,"#define SHADER_NAME "+t.shaderName,M,t.useFog&&t.fog?"#define USE_FOG":"",t.useFog&&t.fogExp2?"#define FOG_EXP2":"",t.alphaToCoverage?"#define ALPHA_TO_COVERAGE":"",t.map?"#define USE_MAP":"",t.matcap?"#define USE_MATCAP":"",t.envMap?"#define USE_ENVMAP":"",t.envMap?"#define "+d:"",t.envMap?"#define "+m:"",t.envMap?"#define "+_:"",g?"#define CUBEUV_TEXEL_WIDTH "+g.texelWidth:"",g?"#define CUBEUV_TEXEL_HEIGHT "+g.texelHeight:"",g?"#define CUBEUV_MAX_MIP "+g.maxMip+".0":"",t.lightMap?"#define USE_LIGHTMAP":"",t.aoMap?"#define USE_AOMAP":"",t.bumpMap?"#define USE_BUMPMAP":"",t.normalMap?"#define USE_NORMALMAP":"",t.normalMapObjectSpace?"#define USE_NORMALMAP_OBJECTSPACE":"",t.normalMapTangentSpace?"#define USE_NORMALMAP_TANGENTSPACE":"",t.emissiveMap?"#define USE_EMISSIVEMAP":"",t.anisotropy?"#define USE_ANISOTROPY":"",t.anisotropyMap?"#define USE_ANISOTROPYMAP":"",t.clearcoat?"#define USE_CLEARCOAT":"",t.clearcoatMap?"#define USE_CLEARCOATMAP":"",t.clearcoatRoughnessMap?"#define USE_CLEARCOAT_ROUGHNESSMAP":"",t.clearcoatNormalMap?"#define USE_CLEARCOAT_NORMALMAP":"",t.dispersion?"#define USE_DISPERSION":"",t.iridescence?"#define USE_IRIDESCENCE":"",t.iridescenceMap?"#define USE_IRIDESCENCEMAP":"",t.iridescenceThicknessMap?"#define USE_IRIDESCENCE_THICKNESSMAP":"",t.specularMap?"#define USE_SPECULARMAP":"",t.specularColorMap?"#define USE_SPECULAR_COLORMAP":"",t.specularIntensityMap?"#define USE_SPECULAR_INTENSITYMAP":"",t.roughnessMap?"#define USE_ROUGHNESSMAP":"",t.metalnessMap?"#define USE_METALNESSMAP":"",t.alphaMap?"#define USE_ALPHAMAP":"",t.alphaTest?"#define USE_ALPHATEST":"",t.alphaHash?"#define USE_ALPHAHASH":"",t.sheen?"#define USE_SHEEN":"",t.sheenColorMap?"#define USE_SHEEN_COLORMAP":"",t.sheenRoughnessMap?"#define USE_SHEEN_ROUGHNESSMAP":"",t.transmission?"#define USE_TRANSMISSION":"",t.transmissionMap?"#define USE_TRANSMISSIONMAP":"",t.thicknessMap?"#define USE_THICKNESSMAP":"",t.vertexTangents&&t.flatShading===!1?"#define USE_TANGENT":"",t.vertexColors||t.instancingColor||t.batchingColor?"#define USE_COLOR":"",t.vertexAlphas?"#define USE_COLOR_ALPHA":"",t.vertexUv1s?"#define USE_UV1":"",t.vertexUv2s?"#define USE_UV2":"",t.vertexUv3s?"#define USE_UV3":"",t.pointsUvs?"#define USE_POINTS_UV":"",t.gradientMap?"#define USE_GRADIENTMAP":"",t.flatShading?"#define FLAT_SHADED":"",t.doubleSided?"#define DOUBLE_SIDED":"",t.flipSided?"#define FLIP_SIDED":"",t.shadowMapEnabled?"#define USE_SHADOWMAP":"",t.shadowMapEnabled?"#define "+h:"",t.premultipliedAlpha?"#define PREMULTIPLIED_ALPHA":"",t.numLightProbes>0?"#define USE_LIGHT_PROBES":"",t.decodeVideoTexture?"#define DECODE_VIDEO_TEXTURE":"",t.logarithmicDepthBuffer?"#define USE_LOGDEPTHBUF":"","uniform mat4 viewMatrix;","uniform vec3 cameraPosition;","uniform bool isOrthographic;",t.toneMapping!==Lr?"#define TONE_MAPPING":"",t.toneMapping!==Lr?dt.tonemapping_pars_fragment:"",t.toneMapping!==Lr?Fw("toneMapping",t.toneMapping):"",t.dithering?"#define DITHERING":"",t.opaque?"#define OPAQUE":"",dt.colorspace_pars_fragment,Uw("linearToOutputTexel",t.outputColorSpace),Ow(),t.useDepthPacking?"#define DEPTH_PACKING "+t.depthPacking:"",`
`].filter(ga).join(`
`)),u=od(u),u=C0(u,t),u=R0(u,t),c=od(c),c=C0(c,t),c=R0(c,t),u=P0(u),c=P0(c),t.isRawShaderMaterial!==!0&&(b=`#version 300 es
`,y=[S,"#define attribute in","#define varying out","#define texture2D texture"].join(`
`)+`
`+y,x=["#define varying in",t.glslVersion===Xg?"":"layout(location = 0) out highp vec4 pc_fragColor;",t.glslVersion===Xg?"":"#define gl_FragColor pc_fragColor","#define gl_FragDepthEXT gl_FragDepth","#define texture2D texture","#define textureCube texture","#define texture2DProj textureProj","#define texture2DLodEXT textureLod","#define texture2DProjLodEXT textureProjLod","#define textureCubeLodEXT textureLod","#define texture2DGradEXT textureGrad","#define texture2DProjGradEXT textureProjGrad","#define textureCubeGradEXT textureGrad"].join(`
`)+`
`+x);const T=b+y+u,P=b+x+c,H=T0(o,o.VERTEX_SHADER,T),D=T0(o,o.FRAGMENT_SHADER,P);o.attachShader(E,H),o.attachShader(E,D),t.index0AttributeName!==void 0?o.bindAttribLocation(E,0,t.index0AttributeName):t.morphTargets===!0&&o.bindAttribLocation(E,0,"position"),o.linkProgram(E);function N(U){if(s.debug.checkShaderErrors){const X=o.getProgramInfoLog(E).trim(),k=o.getShaderInfoLog(H).trim(),Y=o.getShaderInfoLog(D).trim();let ae=!0,te=!0;if(o.getProgramParameter(E,o.LINK_STATUS)===!1)if(ae=!1,typeof s.debug.onShaderError=="function")s.debug.onShaderError(o,E,H,D);else{const ce=A0(o,H,"vertex"),G=A0(o,D,"fragment");console.error("THREE.WebGLProgram: Shader Error "+o.getError()+" - VALIDATE_STATUS "+o.getProgramParameter(E,o.VALIDATE_STATUS)+`

Material Name: `+U.name+`
Material Type: `+U.type+`

Program Info Log: `+X+`
`+ce+`
`+G)}else X!==""?console.warn("THREE.WebGLProgram: Program Info Log:",X):(k===""||Y==="")&&(te=!1);te&&(U.diagnostics={runnable:ae,programLog:X,vertexShader:{log:k,prefix:y},fragmentShader:{log:Y,prefix:x}})}o.deleteShader(H),o.deleteShader(D),z=new Mu(o,E),C=kw(o,E)}let z;this.getUniforms=function(){return z===void 0&&N(this),z};let C;this.getAttributes=function(){return C===void 0&&N(this),C};let w=t.rendererExtensionParallelShaderCompile===!1;return this.isReady=function(){return w===!1&&(w=o.getProgramParameter(E,Lw)),w},this.destroy=function(){i.releaseStatesOfProgram(this),o.deleteProgram(E),this.program=void 0},this.type=t.shaderType,this.name=t.shaderName,this.id=Dw++,this.cacheKey=e,this.usedTimes=1,this.program=E,this.vertexShader=H,this.fragmentShader=D,this}let Jw=0;class Qw{constructor(){this.shaderCache=new Map,this.materialCache=new Map}update(e){const t=e.vertexShader,i=e.fragmentShader,o=this._getShaderStage(t),a=this._getShaderStage(i),u=this._getShaderCacheForMaterial(e);return u.has(o)===!1&&(u.add(o),o.usedTimes++),u.has(a)===!1&&(u.add(a),a.usedTimes++),this}remove(e){const t=this.materialCache.get(e);for(const i of t)i.usedTimes--,i.usedTimes===0&&this.shaderCache.delete(i.code);return this.materialCache.delete(e),this}getVertexShaderID(e){return this._getShaderStage(e.vertexShader).id}getFragmentShaderID(e){return this._getShaderStage(e.fragmentShader).id}dispose(){this.shaderCache.clear(),this.materialCache.clear()}_getShaderCacheForMaterial(e){const t=this.materialCache;let i=t.get(e);return i===void 0&&(i=new Set,t.set(e,i)),i}_getShaderStage(e){const t=this.shaderCache;let i=t.get(e);return i===void 0&&(i=new eT(e),t.set(e,i)),i}}class eT{constructor(e){this.id=Jw++,this.code=e,this.usedTimes=0}}function tT(s,e,t,i,o,a,u){const c=new A_,h=new Qw,d=new Set,m=[],_=o.logarithmicDepthBuffer,g=o.vertexTextures;let S=o.precision;const M={MeshDepthMaterial:"depth",MeshDistanceMaterial:"distanceRGBA",MeshNormalMaterial:"normal",MeshBasicMaterial:"basic",MeshLambertMaterial:"lambert",MeshPhongMaterial:"phong",MeshToonMaterial:"toon",MeshStandardMaterial:"physical",MeshPhysicalMaterial:"physical",MeshMatcapMaterial:"matcap",LineBasicMaterial:"basic",LineDashedMaterial:"dashed",PointsMaterial:"points",ShadowMaterial:"shadow",SpriteMaterial:"sprite"};function E(C){return d.add(C),C===0?"uv":`uv${C}`}function y(C,w,U,X,k){const Y=X.fog,ae=k.geometry,te=C.isMeshStandardMaterial?X.environment:null,ce=(C.isMeshStandardMaterial?t:e).get(C.envMap||te),G=ce&&ce.mapping===Du?ce.image.height:null,he=M[C.type];C.precision!==null&&(S=o.getMaxPrecision(C.precision),S!==C.precision&&console.warn("THREE.WebGLProgram.getParameters:",C.precision,"not supported, using",S,"instead."));const re=ae.morphAttributes.position||ae.morphAttributes.normal||ae.morphAttributes.color,O=re!==void 0?re.length:0;let ie=0;ae.morphAttributes.position!==void 0&&(ie=1),ae.morphAttributes.normal!==void 0&&(ie=2),ae.morphAttributes.color!==void 0&&(ie=3);let Ae,Z,ne,xe;if(he){const St=Ei[he];Ae=St.vertexShader,Z=St.fragmentShader}else Ae=C.vertexShader,Z=C.fragmentShader,h.update(C),ne=h.getVertexShaderID(C),xe=h.getFragmentShaderID(C);const de=s.getRenderTarget(),ee=k.isInstancedMesh===!0,we=k.isBatchedMesh===!0,Ue=!!C.map,tt=!!C.matcap,I=!!ce,Te=!!C.aoMap,Se=!!C.lightMap,Oe=!!C.bumpMap,pe=!!C.normalMap,et=!!C.displacementMap,Be=!!C.emissiveMap,We=!!C.metalnessMap,F=!!C.roughnessMap,R=C.anisotropy>0,oe=C.clearcoat>0,ve=C.dispersion>0,_e=C.iridescence>0,ye=C.sheen>0,Qe=C.transmission>0,Fe=R&&!!C.anisotropyMap,He=oe&&!!C.clearcoatMap,ut=oe&&!!C.clearcoatNormalMap,Re=oe&&!!C.clearcoatRoughnessMap,ke=_e&&!!C.iridescenceMap,_t=_e&&!!C.iridescenceThicknessMap,at=ye&&!!C.sheenColorMap,Xe=ye&&!!C.sheenRoughnessMap,ct=!!C.specularMap,ft=!!C.specularColorMap,bt=!!C.specularIntensityMap,q=Qe&&!!C.transmissionMap,Le=Qe&&!!C.thicknessMap,me=!!C.gradientMap,ge=!!C.alphaMap,Ne=C.alphaTest>0,rt=!!C.alphaHash,vt=!!C.extensions;let Ht=Lr;C.toneMapped&&(de===null||de.isXRRenderTarget===!0)&&(Ht=s.toneMapping);const qt={shaderID:he,shaderType:C.type,shaderName:C.name,vertexShader:Ae,fragmentShader:Z,defines:C.defines,customVertexShaderID:ne,customFragmentShaderID:xe,isRawShaderMaterial:C.isRawShaderMaterial===!0,glslVersion:C.glslVersion,precision:S,batching:we,batchingColor:we&&k._colorsTexture!==null,instancing:ee,instancingColor:ee&&k.instanceColor!==null,instancingMorph:ee&&k.morphTexture!==null,supportsVertexTextures:g,outputColorSpace:de===null?s.outputColorSpace:de.isXRRenderTarget===!0?de.texture.colorSpace:Ir,alphaToCoverage:!!C.alphaToCoverage,map:Ue,matcap:tt,envMap:I,envMapMode:I&&ce.mapping,envMapCubeUVHeight:G,aoMap:Te,lightMap:Se,bumpMap:Oe,normalMap:pe,displacementMap:g&&et,emissiveMap:Be,normalMapObjectSpace:pe&&C.normalMapType===uS,normalMapTangentSpace:pe&&C.normalMapType===S_,metalnessMap:We,roughnessMap:F,anisotropy:R,anisotropyMap:Fe,clearcoat:oe,clearcoatMap:He,clearcoatNormalMap:ut,clearcoatRoughnessMap:Re,dispersion:ve,iridescence:_e,iridescenceMap:ke,iridescenceThicknessMap:_t,sheen:ye,sheenColorMap:at,sheenRoughnessMap:Xe,specularMap:ct,specularColorMap:ft,specularIntensityMap:bt,transmission:Qe,transmissionMap:q,thicknessMap:Le,gradientMap:me,opaque:C.transparent===!1&&C.blending===po&&C.alphaToCoverage===!1,alphaMap:ge,alphaTest:Ne,alphaHash:rt,combine:C.combine,mapUv:Ue&&E(C.map.channel),aoMapUv:Te&&E(C.aoMap.channel),lightMapUv:Se&&E(C.lightMap.channel),bumpMapUv:Oe&&E(C.bumpMap.channel),normalMapUv:pe&&E(C.normalMap.channel),displacementMapUv:et&&E(C.displacementMap.channel),emissiveMapUv:Be&&E(C.emissiveMap.channel),metalnessMapUv:We&&E(C.metalnessMap.channel),roughnessMapUv:F&&E(C.roughnessMap.channel),anisotropyMapUv:Fe&&E(C.anisotropyMap.channel),clearcoatMapUv:He&&E(C.clearcoatMap.channel),clearcoatNormalMapUv:ut&&E(C.clearcoatNormalMap.channel),clearcoatRoughnessMapUv:Re&&E(C.clearcoatRoughnessMap.channel),iridescenceMapUv:ke&&E(C.iridescenceMap.channel),iridescenceThicknessMapUv:_t&&E(C.iridescenceThicknessMap.channel),sheenColorMapUv:at&&E(C.sheenColorMap.channel),sheenRoughnessMapUv:Xe&&E(C.sheenRoughnessMap.channel),specularMapUv:ct&&E(C.specularMap.channel),specularColorMapUv:ft&&E(C.specularColorMap.channel),specularIntensityMapUv:bt&&E(C.specularIntensityMap.channel),transmissionMapUv:q&&E(C.transmissionMap.channel),thicknessMapUv:Le&&E(C.thicknessMap.channel),alphaMapUv:ge&&E(C.alphaMap.channel),vertexTangents:!!ae.attributes.tangent&&(pe||R),vertexColors:C.vertexColors,vertexAlphas:C.vertexColors===!0&&!!ae.attributes.color&&ae.attributes.color.itemSize===4,pointsUvs:k.isPoints===!0&&!!ae.attributes.uv&&(Ue||ge),fog:!!Y,useFog:C.fog===!0,fogExp2:!!Y&&Y.isFogExp2,flatShading:C.flatShading===!0,sizeAttenuation:C.sizeAttenuation===!0,logarithmicDepthBuffer:_,skinning:k.isSkinnedMesh===!0,morphTargets:ae.morphAttributes.position!==void 0,morphNormals:ae.morphAttributes.normal!==void 0,morphColors:ae.morphAttributes.color!==void 0,morphTargetsCount:O,morphTextureStride:ie,numDirLights:w.directional.length,numPointLights:w.point.length,numSpotLights:w.spot.length,numSpotLightMaps:w.spotLightMap.length,numRectAreaLights:w.rectArea.length,numHemiLights:w.hemi.length,numDirLightShadows:w.directionalShadowMap.length,numPointLightShadows:w.pointShadowMap.length,numSpotLightShadows:w.spotShadowMap.length,numSpotLightShadowsWithMaps:w.numSpotLightShadowsWithMaps,numLightProbes:w.numLightProbes,numClippingPlanes:u.numPlanes,numClipIntersection:u.numIntersection,dithering:C.dithering,shadowMapEnabled:s.shadowMap.enabled&&U.length>0,shadowMapType:s.shadowMap.type,toneMapping:Ht,decodeVideoTexture:Ue&&C.map.isVideoTexture===!0&&Tt.getTransfer(C.map.colorSpace)===Ft,premultipliedAlpha:C.premultipliedAlpha,doubleSided:C.side===wi,flipSided:C.side===Bn,useDepthPacking:C.depthPacking>=0,depthPacking:C.depthPacking||0,index0AttributeName:C.index0AttributeName,extensionClipCullDistance:vt&&C.extensions.clipCullDistance===!0&&i.has("WEBGL_clip_cull_distance"),extensionMultiDraw:(vt&&C.extensions.multiDraw===!0||we)&&i.has("WEBGL_multi_draw"),rendererExtensionParallelShaderCompile:i.has("KHR_parallel_shader_compile"),customProgramCacheKey:C.customProgramCacheKey()};return qt.vertexUv1s=d.has(1),qt.vertexUv2s=d.has(2),qt.vertexUv3s=d.has(3),d.clear(),qt}function x(C){const w=[];if(C.shaderID?w.push(C.shaderID):(w.push(C.customVertexShaderID),w.push(C.customFragmentShaderID)),C.defines!==void 0)for(const U in C.defines)w.push(U),w.push(C.defines[U]);return C.isRawShaderMaterial===!1&&(b(w,C),T(w,C),w.push(s.outputColorSpace)),w.push(C.customProgramCacheKey),w.join()}function b(C,w){C.push(w.precision),C.push(w.outputColorSpace),C.push(w.envMapMode),C.push(w.envMapCubeUVHeight),C.push(w.mapUv),C.push(w.alphaMapUv),C.push(w.lightMapUv),C.push(w.aoMapUv),C.push(w.bumpMapUv),C.push(w.normalMapUv),C.push(w.displacementMapUv),C.push(w.emissiveMapUv),C.push(w.metalnessMapUv),C.push(w.roughnessMapUv),C.push(w.anisotropyMapUv),C.push(w.clearcoatMapUv),C.push(w.clearcoatNormalMapUv),C.push(w.clearcoatRoughnessMapUv),C.push(w.iridescenceMapUv),C.push(w.iridescenceThicknessMapUv),C.push(w.sheenColorMapUv),C.push(w.sheenRoughnessMapUv),C.push(w.specularMapUv),C.push(w.specularColorMapUv),C.push(w.specularIntensityMapUv),C.push(w.transmissionMapUv),C.push(w.thicknessMapUv),C.push(w.combine),C.push(w.fogExp2),C.push(w.sizeAttenuation),C.push(w.morphTargetsCount),C.push(w.morphAttributeCount),C.push(w.numDirLights),C.push(w.numPointLights),C.push(w.numSpotLights),C.push(w.numSpotLightMaps),C.push(w.numHemiLights),C.push(w.numRectAreaLights),C.push(w.numDirLightShadows),C.push(w.numPointLightShadows),C.push(w.numSpotLightShadows),C.push(w.numSpotLightShadowsWithMaps),C.push(w.numLightProbes),C.push(w.shadowMapType),C.push(w.toneMapping),C.push(w.numClippingPlanes),C.push(w.numClipIntersection),C.push(w.depthPacking)}function T(C,w){c.disableAll(),w.supportsVertexTextures&&c.enable(0),w.instancing&&c.enable(1),w.instancingColor&&c.enable(2),w.instancingMorph&&c.enable(3),w.matcap&&c.enable(4),w.envMap&&c.enable(5),w.normalMapObjectSpace&&c.enable(6),w.normalMapTangentSpace&&c.enable(7),w.clearcoat&&c.enable(8),w.iridescence&&c.enable(9),w.alphaTest&&c.enable(10),w.vertexColors&&c.enable(11),w.vertexAlphas&&c.enable(12),w.vertexUv1s&&c.enable(13),w.vertexUv2s&&c.enable(14),w.vertexUv3s&&c.enable(15),w.vertexTangents&&c.enable(16),w.anisotropy&&c.enable(17),w.alphaHash&&c.enable(18),w.batching&&c.enable(19),w.dispersion&&c.enable(20),w.batchingColor&&c.enable(21),C.push(c.mask),c.disableAll(),w.fog&&c.enable(0),w.useFog&&c.enable(1),w.flatShading&&c.enable(2),w.logarithmicDepthBuffer&&c.enable(3),w.skinning&&c.enable(4),w.morphTargets&&c.enable(5),w.morphNormals&&c.enable(6),w.morphColors&&c.enable(7),w.premultipliedAlpha&&c.enable(8),w.shadowMapEnabled&&c.enable(9),w.doubleSided&&c.enable(10),w.flipSided&&c.enable(11),w.useDepthPacking&&c.enable(12),w.dithering&&c.enable(13),w.transmission&&c.enable(14),w.sheen&&c.enable(15),w.opaque&&c.enable(16),w.pointsUvs&&c.enable(17),w.decodeVideoTexture&&c.enable(18),w.alphaToCoverage&&c.enable(19),C.push(c.mask)}function P(C){const w=M[C.type];let U;if(w){const X=Ei[w];U=BS.clone(X.uniforms)}else U=C.uniforms;return U}function H(C,w){let U;for(let X=0,k=m.length;X<k;X++){const Y=m[X];if(Y.cacheKey===w){U=Y,++U.usedTimes;break}}return U===void 0&&(U=new Zw(s,w,C,a),m.push(U)),U}function D(C){if(--C.usedTimes===0){const w=m.indexOf(C);m[w]=m[m.length-1],m.pop(),C.destroy()}}function N(C){h.remove(C)}function z(){h.dispose()}return{getParameters:y,getProgramCacheKey:x,getUniforms:P,acquireProgram:H,releaseProgram:D,releaseShaderCache:N,programs:m,dispose:z}}function nT(){let s=new WeakMap;function e(u){return s.has(u)}function t(u){let c=s.get(u);return c===void 0&&(c={},s.set(u,c)),c}function i(u){s.delete(u)}function o(u,c,h){s.get(u)[c]=h}function a(){s=new WeakMap}return{has:e,get:t,remove:i,update:o,dispose:a}}function iT(s,e){return s.groupOrder!==e.groupOrder?s.groupOrder-e.groupOrder:s.renderOrder!==e.renderOrder?s.renderOrder-e.renderOrder:s.material.id!==e.material.id?s.material.id-e.material.id:s.z!==e.z?s.z-e.z:s.id-e.id}function L0(s,e){return s.groupOrder!==e.groupOrder?s.groupOrder-e.groupOrder:s.renderOrder!==e.renderOrder?s.renderOrder-e.renderOrder:s.z!==e.z?e.z-s.z:s.id-e.id}function D0(){const s=[];let e=0;const t=[],i=[],o=[];function a(){e=0,t.length=0,i.length=0,o.length=0}function u(_,g,S,M,E,y){let x=s[e];return x===void 0?(x={id:_.id,object:_,geometry:g,material:S,groupOrder:M,renderOrder:_.renderOrder,z:E,group:y},s[e]=x):(x.id=_.id,x.object=_,x.geometry=g,x.material=S,x.groupOrder=M,x.renderOrder=_.renderOrder,x.z=E,x.group=y),e++,x}function c(_,g,S,M,E,y){const x=u(_,g,S,M,E,y);S.transmission>0?i.push(x):S.transparent===!0?o.push(x):t.push(x)}function h(_,g,S,M,E,y){const x=u(_,g,S,M,E,y);S.transmission>0?i.unshift(x):S.transparent===!0?o.unshift(x):t.unshift(x)}function d(_,g){t.length>1&&t.sort(_||iT),i.length>1&&i.sort(g||L0),o.length>1&&o.sort(g||L0)}function m(){for(let _=e,g=s.length;_<g;_++){const S=s[_];if(S.id===null)break;S.id=null,S.object=null,S.geometry=null,S.material=null,S.group=null}}return{opaque:t,transmissive:i,transparent:o,init:a,push:c,unshift:h,finish:m,sort:d}}function rT(){let s=new WeakMap;function e(i,o){const a=s.get(i);let u;return a===void 0?(u=new D0,s.set(i,[u])):o>=a.length?(u=new D0,a.push(u)):u=a[o],u}function t(){s=new WeakMap}return{get:e,dispose:t}}function sT(){const s={};return{get:function(e){if(s[e.id]!==void 0)return s[e.id];let t;switch(e.type){case"DirectionalLight":t={direction:new W,color:new yt};break;case"SpotLight":t={position:new W,direction:new W,color:new yt,distance:0,coneCos:0,penumbraCos:0,decay:0};break;case"PointLight":t={position:new W,color:new yt,distance:0,decay:0};break;case"HemisphereLight":t={direction:new W,skyColor:new yt,groundColor:new yt};break;case"RectAreaLight":t={color:new yt,position:new W,halfWidth:new W,halfHeight:new W};break}return s[e.id]=t,t}}}function oT(){const s={};return{get:function(e){if(s[e.id]!==void 0)return s[e.id];let t;switch(e.type){case"DirectionalLight":t={shadowIntensity:1,shadowBias:0,shadowNormalBias:0,shadowRadius:1,shadowMapSize:new Pe};break;case"SpotLight":t={shadowIntensity:1,shadowBias:0,shadowNormalBias:0,shadowRadius:1,shadowMapSize:new Pe};break;case"PointLight":t={shadowIntensity:1,shadowBias:0,shadowNormalBias:0,shadowRadius:1,shadowMapSize:new Pe,shadowCameraNear:1,shadowCameraFar:1e3};break}return s[e.id]=t,t}}}let aT=0;function lT(s,e){return(e.castShadow?2:0)-(s.castShadow?2:0)+(e.map?1:0)-(s.map?1:0)}function uT(s){const e=new sT,t=oT(),i={version:0,hash:{directionalLength:-1,pointLength:-1,spotLength:-1,rectAreaLength:-1,hemiLength:-1,numDirectionalShadows:-1,numPointShadows:-1,numSpotShadows:-1,numSpotMaps:-1,numLightProbes:-1},ambient:[0,0,0],probe:[],directional:[],directionalShadow:[],directionalShadowMap:[],directionalShadowMatrix:[],spot:[],spotLightMap:[],spotShadow:[],spotShadowMap:[],spotLightMatrix:[],rectArea:[],rectAreaLTC1:null,rectAreaLTC2:null,point:[],pointShadow:[],pointShadowMap:[],pointShadowMatrix:[],hemi:[],numSpotLightShadowsWithMaps:0,numLightProbes:0};for(let d=0;d<9;d++)i.probe.push(new W);const o=new W,a=new kt,u=new kt;function c(d){let m=0,_=0,g=0;for(let C=0;C<9;C++)i.probe[C].set(0,0,0);let S=0,M=0,E=0,y=0,x=0,b=0,T=0,P=0,H=0,D=0,N=0;d.sort(lT);for(let C=0,w=d.length;C<w;C++){const U=d[C],X=U.color,k=U.intensity,Y=U.distance,ae=U.shadow&&U.shadow.map?U.shadow.map.texture:null;if(U.isAmbientLight)m+=X.r*k,_+=X.g*k,g+=X.b*k;else if(U.isLightProbe){for(let te=0;te<9;te++)i.probe[te].addScaledVector(U.sh.coefficients[te],k);N++}else if(U.isDirectionalLight){const te=e.get(U);if(te.color.copy(U.color).multiplyScalar(U.intensity),U.castShadow){const ce=U.shadow,G=t.get(U);G.shadowIntensity=ce.intensity,G.shadowBias=ce.bias,G.shadowNormalBias=ce.normalBias,G.shadowRadius=ce.radius,G.shadowMapSize=ce.mapSize,i.directionalShadow[S]=G,i.directionalShadowMap[S]=ae,i.directionalShadowMatrix[S]=U.shadow.matrix,b++}i.directional[S]=te,S++}else if(U.isSpotLight){const te=e.get(U);te.position.setFromMatrixPosition(U.matrixWorld),te.color.copy(X).multiplyScalar(k),te.distance=Y,te.coneCos=Math.cos(U.angle),te.penumbraCos=Math.cos(U.angle*(1-U.penumbra)),te.decay=U.decay,i.spot[E]=te;const ce=U.shadow;if(U.map&&(i.spotLightMap[H]=U.map,H++,ce.updateMatrices(U),U.castShadow&&D++),i.spotLightMatrix[E]=ce.matrix,U.castShadow){const G=t.get(U);G.shadowIntensity=ce.intensity,G.shadowBias=ce.bias,G.shadowNormalBias=ce.normalBias,G.shadowRadius=ce.radius,G.shadowMapSize=ce.mapSize,i.spotShadow[E]=G,i.spotShadowMap[E]=ae,P++}E++}else if(U.isRectAreaLight){const te=e.get(U);te.color.copy(X).multiplyScalar(k),te.halfWidth.set(U.width*.5,0,0),te.halfHeight.set(0,U.height*.5,0),i.rectArea[y]=te,y++}else if(U.isPointLight){const te=e.get(U);if(te.color.copy(U.color).multiplyScalar(U.intensity),te.distance=U.distance,te.decay=U.decay,U.castShadow){const ce=U.shadow,G=t.get(U);G.shadowIntensity=ce.intensity,G.shadowBias=ce.bias,G.shadowNormalBias=ce.normalBias,G.shadowRadius=ce.radius,G.shadowMapSize=ce.mapSize,G.shadowCameraNear=ce.camera.near,G.shadowCameraFar=ce.camera.far,i.pointShadow[M]=G,i.pointShadowMap[M]=ae,i.pointShadowMatrix[M]=U.shadow.matrix,T++}i.point[M]=te,M++}else if(U.isHemisphereLight){const te=e.get(U);te.skyColor.copy(U.color).multiplyScalar(k),te.groundColor.copy(U.groundColor).multiplyScalar(k),i.hemi[x]=te,x++}}y>0&&(s.has("OES_texture_float_linear")===!0?(i.rectAreaLTC1=ze.LTC_FLOAT_1,i.rectAreaLTC2=ze.LTC_FLOAT_2):(i.rectAreaLTC1=ze.LTC_HALF_1,i.rectAreaLTC2=ze.LTC_HALF_2)),i.ambient[0]=m,i.ambient[1]=_,i.ambient[2]=g;const z=i.hash;(z.directionalLength!==S||z.pointLength!==M||z.spotLength!==E||z.rectAreaLength!==y||z.hemiLength!==x||z.numDirectionalShadows!==b||z.numPointShadows!==T||z.numSpotShadows!==P||z.numSpotMaps!==H||z.numLightProbes!==N)&&(i.directional.length=S,i.spot.length=E,i.rectArea.length=y,i.point.length=M,i.hemi.length=x,i.directionalShadow.length=b,i.directionalShadowMap.length=b,i.pointShadow.length=T,i.pointShadowMap.length=T,i.spotShadow.length=P,i.spotShadowMap.length=P,i.directionalShadowMatrix.length=b,i.pointShadowMatrix.length=T,i.spotLightMatrix.length=P+H-D,i.spotLightMap.length=H,i.numSpotLightShadowsWithMaps=D,i.numLightProbes=N,z.directionalLength=S,z.pointLength=M,z.spotLength=E,z.rectAreaLength=y,z.hemiLength=x,z.numDirectionalShadows=b,z.numPointShadows=T,z.numSpotShadows=P,z.numSpotMaps=H,z.numLightProbes=N,i.version=aT++)}function h(d,m){let _=0,g=0,S=0,M=0,E=0;const y=m.matrixWorldInverse;for(let x=0,b=d.length;x<b;x++){const T=d[x];if(T.isDirectionalLight){const P=i.directional[_];P.direction.setFromMatrixPosition(T.matrixWorld),o.setFromMatrixPosition(T.target.matrixWorld),P.direction.sub(o),P.direction.transformDirection(y),_++}else if(T.isSpotLight){const P=i.spot[S];P.position.setFromMatrixPosition(T.matrixWorld),P.position.applyMatrix4(y),P.direction.setFromMatrixPosition(T.matrixWorld),o.setFromMatrixPosition(T.target.matrixWorld),P.direction.sub(o),P.direction.transformDirection(y),S++}else if(T.isRectAreaLight){const P=i.rectArea[M];P.position.setFromMatrixPosition(T.matrixWorld),P.position.applyMatrix4(y),u.identity(),a.copy(T.matrixWorld),a.premultiply(y),u.extractRotation(a),P.halfWidth.set(T.width*.5,0,0),P.halfHeight.set(0,T.height*.5,0),P.halfWidth.applyMatrix4(u),P.halfHeight.applyMatrix4(u),M++}else if(T.isPointLight){const P=i.point[g];P.position.setFromMatrixPosition(T.matrixWorld),P.position.applyMatrix4(y),g++}else if(T.isHemisphereLight){const P=i.hemi[E];P.direction.setFromMatrixPosition(T.matrixWorld),P.direction.transformDirection(y),E++}}}return{setup:c,setupView:h,state:i}}function N0(s){const e=new uT(s),t=[],i=[];function o(m){d.camera=m,t.length=0,i.length=0}function a(m){t.push(m)}function u(m){i.push(m)}function c(){e.setup(t)}function h(m){e.setupView(t,m)}const d={lightsArray:t,shadowsArray:i,camera:null,lights:e,transmissionRenderTarget:{}};return{init:o,state:d,setupLights:c,setupLightsView:h,pushLight:a,pushShadow:u}}function cT(s){let e=new WeakMap;function t(o,a=0){const u=e.get(o);let c;return u===void 0?(c=new N0(s),e.set(o,[c])):a>=u.length?(c=new N0(s),u.push(c)):c=u[a],c}function i(){e=new WeakMap}return{get:t,dispose:i}}class fT extends ds{constructor(e){super(),this.isMeshDepthMaterial=!0,this.type="MeshDepthMaterial",this.depthPacking=aS,this.map=null,this.alphaMap=null,this.displacementMap=null,this.displacementScale=1,this.displacementBias=0,this.wireframe=!1,this.wireframeLinewidth=1,this.setValues(e)}copy(e){return super.copy(e),this.depthPacking=e.depthPacking,this.map=e.map,this.alphaMap=e.alphaMap,this.displacementMap=e.displacementMap,this.displacementScale=e.displacementScale,this.displacementBias=e.displacementBias,this.wireframe=e.wireframe,this.wireframeLinewidth=e.wireframeLinewidth,this}}class hT extends ds{constructor(e){super(),this.isMeshDistanceMaterial=!0,this.type="MeshDistanceMaterial",this.map=null,this.alphaMap=null,this.displacementMap=null,this.displacementScale=1,this.displacementBias=0,this.setValues(e)}copy(e){return super.copy(e),this.map=e.map,this.alphaMap=e.alphaMap,this.displacementMap=e.displacementMap,this.displacementScale=e.displacementScale,this.displacementBias=e.displacementBias,this}}const dT=`void main() {
	gl_Position = vec4( position, 1.0 );
}`,pT=`uniform sampler2D shadow_pass;
uniform vec2 resolution;
uniform float radius;
#include <packing>
void main() {
	const float samples = float( VSM_SAMPLES );
	float mean = 0.0;
	float squared_mean = 0.0;
	float uvStride = samples <= 1.0 ? 0.0 : 2.0 / ( samples - 1.0 );
	float uvStart = samples <= 1.0 ? 0.0 : - 1.0;
	for ( float i = 0.0; i < samples; i ++ ) {
		float uvOffset = uvStart + i * uvStride;
		#ifdef HORIZONTAL_PASS
			vec2 distribution = unpackRGBATo2Half( texture2D( shadow_pass, ( gl_FragCoord.xy + vec2( uvOffset, 0.0 ) * radius ) / resolution ) );
			mean += distribution.x;
			squared_mean += distribution.y * distribution.y + distribution.x * distribution.x;
		#else
			float depth = unpackRGBAToDepth( texture2D( shadow_pass, ( gl_FragCoord.xy + vec2( 0.0, uvOffset ) * radius ) / resolution ) );
			mean += depth;
			squared_mean += depth * depth;
		#endif
	}
	mean = mean / samples;
	squared_mean = squared_mean / samples;
	float std_dev = sqrt( squared_mean - mean * mean );
	gl_FragColor = pack2HalfToRGBA( vec2( mean, std_dev ) );
}`;function mT(s,e,t){let i=new Md;const o=new Pe,a=new Pe,u=new Qt,c=new fT({depthPacking:lS}),h=new hT,d={},m=t.maxTextureSize,_={[Dr]:Bn,[Bn]:Dr,[wi]:wi},g=new Nr({defines:{VSM_SAMPLES:8},uniforms:{shadow_pass:{value:null},resolution:{value:new Pe},radius:{value:4}},vertexShader:dT,fragmentShader:pT}),S=g.clone();S.defines.HORIZONTAL_PASS=1;const M=new un;M.setAttribute("position",new gi(new Float32Array([-1,-1,.5,3,-1,.5,-1,3,.5]),3));const E=new zt(M,g),y=this;this.enabled=!1,this.autoUpdate=!0,this.needsUpdate=!1,this.type=l_;let x=this.type;this.render=function(D,N,z){if(y.enabled===!1||y.autoUpdate===!1&&y.needsUpdate===!1||D.length===0)return;const C=s.getRenderTarget(),w=s.getActiveCubeFace(),U=s.getActiveMipmapLevel(),X=s.state;X.setBlending(br),X.buffers.color.setClear(1,1,1,1),X.buffers.depth.setTest(!0),X.setScissorTest(!1);const k=x!==Wi&&this.type===Wi,Y=x===Wi&&this.type!==Wi;for(let ae=0,te=D.length;ae<te;ae++){const ce=D[ae],G=ce.shadow;if(G===void 0){console.warn("THREE.WebGLShadowMap:",ce,"has no shadow.");continue}if(G.autoUpdate===!1&&G.needsUpdate===!1)continue;o.copy(G.mapSize);const he=G.getFrameExtents();if(o.multiply(he),a.copy(G.mapSize),(o.x>m||o.y>m)&&(o.x>m&&(a.x=Math.floor(m/he.x),o.x=a.x*he.x,G.mapSize.x=a.x),o.y>m&&(a.y=Math.floor(m/he.y),o.y=a.y*he.y,G.mapSize.y=a.y)),G.map===null||k===!0||Y===!0){const O=this.type!==Wi?{minFilter:ri,magFilter:ri}:{};G.map!==null&&G.map.dispose(),G.map=new as(o.x,o.y,O),G.map.texture.name=ce.name+".shadowMap",G.camera.updateProjectionMatrix()}s.setRenderTarget(G.map),s.clear();const re=G.getViewportCount();for(let O=0;O<re;O++){const ie=G.getViewport(O);u.set(a.x*ie.x,a.y*ie.y,a.x*ie.z,a.y*ie.w),X.viewport(u),G.updateMatrices(ce,O),i=G.getFrustum(),P(N,z,G.camera,ce,this.type)}G.isPointLightShadow!==!0&&this.type===Wi&&b(G,z),G.needsUpdate=!1}x=this.type,y.needsUpdate=!1,s.setRenderTarget(C,w,U)};function b(D,N){const z=e.update(E);g.defines.VSM_SAMPLES!==D.blurSamples&&(g.defines.VSM_SAMPLES=D.blurSamples,S.defines.VSM_SAMPLES=D.blurSamples,g.needsUpdate=!0,S.needsUpdate=!0),D.mapPass===null&&(D.mapPass=new as(o.x,o.y)),g.uniforms.shadow_pass.value=D.map.texture,g.uniforms.resolution.value=D.mapSize,g.uniforms.radius.value=D.radius,s.setRenderTarget(D.mapPass),s.clear(),s.renderBufferDirect(N,null,z,g,E,null),S.uniforms.shadow_pass.value=D.mapPass.texture,S.uniforms.resolution.value=D.mapSize,S.uniforms.radius.value=D.radius,s.setRenderTarget(D.map),s.clear(),s.renderBufferDirect(N,null,z,S,E,null)}function T(D,N,z,C){let w=null;const U=z.isPointLight===!0?D.customDistanceMaterial:D.customDepthMaterial;if(U!==void 0)w=U;else if(w=z.isPointLight===!0?h:c,s.localClippingEnabled&&N.clipShadows===!0&&Array.isArray(N.clippingPlanes)&&N.clippingPlanes.length!==0||N.displacementMap&&N.displacementScale!==0||N.alphaMap&&N.alphaTest>0||N.map&&N.alphaTest>0){const X=w.uuid,k=N.uuid;let Y=d[X];Y===void 0&&(Y={},d[X]=Y);let ae=Y[k];ae===void 0&&(ae=w.clone(),Y[k]=ae,N.addEventListener("dispose",H)),w=ae}if(w.visible=N.visible,w.wireframe=N.wireframe,C===Wi?w.side=N.shadowSide!==null?N.shadowSide:N.side:w.side=N.shadowSide!==null?N.shadowSide:_[N.side],w.alphaMap=N.alphaMap,w.alphaTest=N.alphaTest,w.map=N.map,w.clipShadows=N.clipShadows,w.clippingPlanes=N.clippingPlanes,w.clipIntersection=N.clipIntersection,w.displacementMap=N.displacementMap,w.displacementScale=N.displacementScale,w.displacementBias=N.displacementBias,w.wireframeLinewidth=N.wireframeLinewidth,w.linewidth=N.linewidth,z.isPointLight===!0&&w.isMeshDistanceMaterial===!0){const X=s.properties.get(w);X.light=z}return w}function P(D,N,z,C,w){if(D.visible===!1)return;if(D.layers.test(N.layers)&&(D.isMesh||D.isLine||D.isPoints)&&(D.castShadow||D.receiveShadow&&w===Wi)&&(!D.frustumCulled||i.intersectsObject(D))){D.modelViewMatrix.multiplyMatrices(z.matrixWorldInverse,D.matrixWorld);const k=e.update(D),Y=D.material;if(Array.isArray(Y)){const ae=k.groups;for(let te=0,ce=ae.length;te<ce;te++){const G=ae[te],he=Y[G.materialIndex];if(he&&he.visible){const re=T(D,he,C,w);D.onBeforeShadow(s,D,N,z,k,re,G),s.renderBufferDirect(z,null,k,re,D,G),D.onAfterShadow(s,D,N,z,k,re,G)}}}else if(Y.visible){const ae=T(D,Y,C,w);D.onBeforeShadow(s,D,N,z,k,ae,null),s.renderBufferDirect(z,null,k,ae,D,null),D.onAfterShadow(s,D,N,z,k,ae,null)}}const X=D.children;for(let k=0,Y=X.length;k<Y;k++)P(X[k],N,z,C,w)}function H(D){D.target.removeEventListener("dispose",H);for(const z in d){const C=d[z],w=D.target.uuid;w in C&&(C[w].dispose(),delete C[w])}}}function gT(s){function e(){let q=!1;const Le=new Qt;let me=null;const ge=new Qt(0,0,0,0);return{setMask:function(Ne){me!==Ne&&!q&&(s.colorMask(Ne,Ne,Ne,Ne),me=Ne)},setLocked:function(Ne){q=Ne},setClear:function(Ne,rt,vt,Ht,qt){qt===!0&&(Ne*=Ht,rt*=Ht,vt*=Ht),Le.set(Ne,rt,vt,Ht),ge.equals(Le)===!1&&(s.clearColor(Ne,rt,vt,Ht),ge.copy(Le))},reset:function(){q=!1,me=null,ge.set(-1,0,0,0)}}}function t(){let q=!1,Le=null,me=null,ge=null;return{setTest:function(Ne){Ne?xe(s.DEPTH_TEST):de(s.DEPTH_TEST)},setMask:function(Ne){Le!==Ne&&!q&&(s.depthMask(Ne),Le=Ne)},setFunc:function(Ne){if(me!==Ne){switch(Ne){case Wx:s.depthFunc(s.NEVER);break;case Xx:s.depthFunc(s.ALWAYS);break;case jx:s.depthFunc(s.LESS);break;case Eu:s.depthFunc(s.LEQUAL);break;case qx:s.depthFunc(s.EQUAL);break;case Yx:s.depthFunc(s.GEQUAL);break;case Kx:s.depthFunc(s.GREATER);break;case $x:s.depthFunc(s.NOTEQUAL);break;default:s.depthFunc(s.LEQUAL)}me=Ne}},setLocked:function(Ne){q=Ne},setClear:function(Ne){ge!==Ne&&(s.clearDepth(Ne),ge=Ne)},reset:function(){q=!1,Le=null,me=null,ge=null}}}function i(){let q=!1,Le=null,me=null,ge=null,Ne=null,rt=null,vt=null,Ht=null,qt=null;return{setTest:function(St){q||(St?xe(s.STENCIL_TEST):de(s.STENCIL_TEST))},setMask:function(St){Le!==St&&!q&&(s.stencilMask(St),Le=St)},setFunc:function(St,zn,kn){(me!==St||ge!==zn||Ne!==kn)&&(s.stencilFunc(St,zn,kn),me=St,ge=zn,Ne=kn)},setOp:function(St,zn,kn){(rt!==St||vt!==zn||Ht!==kn)&&(s.stencilOp(St,zn,kn),rt=St,vt=zn,Ht=kn)},setLocked:function(St){q=St},setClear:function(St){qt!==St&&(s.clearStencil(St),qt=St)},reset:function(){q=!1,Le=null,me=null,ge=null,Ne=null,rt=null,vt=null,Ht=null,qt=null}}}const o=new e,a=new t,u=new i,c=new WeakMap,h=new WeakMap;let d={},m={},_=new WeakMap,g=[],S=null,M=!1,E=null,y=null,x=null,b=null,T=null,P=null,H=null,D=new yt(0,0,0),N=0,z=!1,C=null,w=null,U=null,X=null,k=null;const Y=s.getParameter(s.MAX_COMBINED_TEXTURE_IMAGE_UNITS);let ae=!1,te=0;const ce=s.getParameter(s.VERSION);ce.indexOf("WebGL")!==-1?(te=parseFloat(/^WebGL (\d)/.exec(ce)[1]),ae=te>=1):ce.indexOf("OpenGL ES")!==-1&&(te=parseFloat(/^OpenGL ES (\d)/.exec(ce)[1]),ae=te>=2);let G=null,he={};const re=s.getParameter(s.SCISSOR_BOX),O=s.getParameter(s.VIEWPORT),ie=new Qt().fromArray(re),Ae=new Qt().fromArray(O);function Z(q,Le,me,ge){const Ne=new Uint8Array(4),rt=s.createTexture();s.bindTexture(q,rt),s.texParameteri(q,s.TEXTURE_MIN_FILTER,s.NEAREST),s.texParameteri(q,s.TEXTURE_MAG_FILTER,s.NEAREST);for(let vt=0;vt<me;vt++)q===s.TEXTURE_3D||q===s.TEXTURE_2D_ARRAY?s.texImage3D(Le,0,s.RGBA,1,1,ge,0,s.RGBA,s.UNSIGNED_BYTE,Ne):s.texImage2D(Le+vt,0,s.RGBA,1,1,0,s.RGBA,s.UNSIGNED_BYTE,Ne);return rt}const ne={};ne[s.TEXTURE_2D]=Z(s.TEXTURE_2D,s.TEXTURE_2D,1),ne[s.TEXTURE_CUBE_MAP]=Z(s.TEXTURE_CUBE_MAP,s.TEXTURE_CUBE_MAP_POSITIVE_X,6),ne[s.TEXTURE_2D_ARRAY]=Z(s.TEXTURE_2D_ARRAY,s.TEXTURE_2D_ARRAY,1,1),ne[s.TEXTURE_3D]=Z(s.TEXTURE_3D,s.TEXTURE_3D,1,1),o.setClear(0,0,0,1),a.setClear(1),u.setClear(0),xe(s.DEPTH_TEST),a.setFunc(Eu),Oe(!1),pe(kg),xe(s.CULL_FACE),Te(br);function xe(q){d[q]!==!0&&(s.enable(q),d[q]=!0)}function de(q){d[q]!==!1&&(s.disable(q),d[q]=!1)}function ee(q,Le){return m[q]!==Le?(s.bindFramebuffer(q,Le),m[q]=Le,q===s.DRAW_FRAMEBUFFER&&(m[s.FRAMEBUFFER]=Le),q===s.FRAMEBUFFER&&(m[s.DRAW_FRAMEBUFFER]=Le),!0):!1}function we(q,Le){let me=g,ge=!1;if(q){me=_.get(Le),me===void 0&&(me=[],_.set(Le,me));const Ne=q.textures;if(me.length!==Ne.length||me[0]!==s.COLOR_ATTACHMENT0){for(let rt=0,vt=Ne.length;rt<vt;rt++)me[rt]=s.COLOR_ATTACHMENT0+rt;me.length=Ne.length,ge=!0}}else me[0]!==s.BACK&&(me[0]=s.BACK,ge=!0);ge&&s.drawBuffers(me)}function Ue(q){return S!==q?(s.useProgram(q),S=q,!0):!1}const tt={[ns]:s.FUNC_ADD,[Cx]:s.FUNC_SUBTRACT,[Rx]:s.FUNC_REVERSE_SUBTRACT};tt[Px]=s.MIN,tt[bx]=s.MAX;const I={[Lx]:s.ZERO,[Dx]:s.ONE,[Nx]:s.SRC_COLOR,[Ah]:s.SRC_ALPHA,[zx]:s.SRC_ALPHA_SATURATE,[Ox]:s.DST_COLOR,[Ux]:s.DST_ALPHA,[Ix]:s.ONE_MINUS_SRC_COLOR,[Ch]:s.ONE_MINUS_SRC_ALPHA,[Bx]:s.ONE_MINUS_DST_COLOR,[Fx]:s.ONE_MINUS_DST_ALPHA,[kx]:s.CONSTANT_COLOR,[Hx]:s.ONE_MINUS_CONSTANT_COLOR,[Vx]:s.CONSTANT_ALPHA,[Gx]:s.ONE_MINUS_CONSTANT_ALPHA};function Te(q,Le,me,ge,Ne,rt,vt,Ht,qt,St){if(q===br){M===!0&&(de(s.BLEND),M=!1);return}if(M===!1&&(xe(s.BLEND),M=!0),q!==Ax){if(q!==E||St!==z){if((y!==ns||T!==ns)&&(s.blendEquation(s.FUNC_ADD),y=ns,T=ns),St)switch(q){case po:s.blendFuncSeparate(s.ONE,s.ONE_MINUS_SRC_ALPHA,s.ONE,s.ONE_MINUS_SRC_ALPHA);break;case Hg:s.blendFunc(s.ONE,s.ONE);break;case Vg:s.blendFuncSeparate(s.ZERO,s.ONE_MINUS_SRC_COLOR,s.ZERO,s.ONE);break;case Gg:s.blendFuncSeparate(s.ZERO,s.SRC_COLOR,s.ZERO,s.SRC_ALPHA);break;default:console.error("THREE.WebGLState: Invalid blending: ",q);break}else switch(q){case po:s.blendFuncSeparate(s.SRC_ALPHA,s.ONE_MINUS_SRC_ALPHA,s.ONE,s.ONE_MINUS_SRC_ALPHA);break;case Hg:s.blendFunc(s.SRC_ALPHA,s.ONE);break;case Vg:s.blendFuncSeparate(s.ZERO,s.ONE_MINUS_SRC_COLOR,s.ZERO,s.ONE);break;case Gg:s.blendFunc(s.ZERO,s.SRC_COLOR);break;default:console.error("THREE.WebGLState: Invalid blending: ",q);break}x=null,b=null,P=null,H=null,D.set(0,0,0),N=0,E=q,z=St}return}Ne=Ne||Le,rt=rt||me,vt=vt||ge,(Le!==y||Ne!==T)&&(s.blendEquationSeparate(tt[Le],tt[Ne]),y=Le,T=Ne),(me!==x||ge!==b||rt!==P||vt!==H)&&(s.blendFuncSeparate(I[me],I[ge],I[rt],I[vt]),x=me,b=ge,P=rt,H=vt),(Ht.equals(D)===!1||qt!==N)&&(s.blendColor(Ht.r,Ht.g,Ht.b,qt),D.copy(Ht),N=qt),E=q,z=!1}function Se(q,Le){q.side===wi?de(s.CULL_FACE):xe(s.CULL_FACE);let me=q.side===Bn;Le&&(me=!me),Oe(me),q.blending===po&&q.transparent===!1?Te(br):Te(q.blending,q.blendEquation,q.blendSrc,q.blendDst,q.blendEquationAlpha,q.blendSrcAlpha,q.blendDstAlpha,q.blendColor,q.blendAlpha,q.premultipliedAlpha),a.setFunc(q.depthFunc),a.setTest(q.depthTest),a.setMask(q.depthWrite),o.setMask(q.colorWrite);const ge=q.stencilWrite;u.setTest(ge),ge&&(u.setMask(q.stencilWriteMask),u.setFunc(q.stencilFunc,q.stencilRef,q.stencilFuncMask),u.setOp(q.stencilFail,q.stencilZFail,q.stencilZPass)),Be(q.polygonOffset,q.polygonOffsetFactor,q.polygonOffsetUnits),q.alphaToCoverage===!0?xe(s.SAMPLE_ALPHA_TO_COVERAGE):de(s.SAMPLE_ALPHA_TO_COVERAGE)}function Oe(q){C!==q&&(q?s.frontFace(s.CW):s.frontFace(s.CCW),C=q)}function pe(q){q!==Ex?(xe(s.CULL_FACE),q!==w&&(q===kg?s.cullFace(s.BACK):q===wx?s.cullFace(s.FRONT):s.cullFace(s.FRONT_AND_BACK))):de(s.CULL_FACE),w=q}function et(q){q!==U&&(ae&&s.lineWidth(q),U=q)}function Be(q,Le,me){q?(xe(s.POLYGON_OFFSET_FILL),(X!==Le||k!==me)&&(s.polygonOffset(Le,me),X=Le,k=me)):de(s.POLYGON_OFFSET_FILL)}function We(q){q?xe(s.SCISSOR_TEST):de(s.SCISSOR_TEST)}function F(q){q===void 0&&(q=s.TEXTURE0+Y-1),G!==q&&(s.activeTexture(q),G=q)}function R(q,Le,me){me===void 0&&(G===null?me=s.TEXTURE0+Y-1:me=G);let ge=he[me];ge===void 0&&(ge={type:void 0,texture:void 0},he[me]=ge),(ge.type!==q||ge.texture!==Le)&&(G!==me&&(s.activeTexture(me),G=me),s.bindTexture(q,Le||ne[q]),ge.type=q,ge.texture=Le)}function oe(){const q=he[G];q!==void 0&&q.type!==void 0&&(s.bindTexture(q.type,null),q.type=void 0,q.texture=void 0)}function ve(){try{s.compressedTexImage2D.apply(s,arguments)}catch(q){console.error("THREE.WebGLState:",q)}}function _e(){try{s.compressedTexImage3D.apply(s,arguments)}catch(q){console.error("THREE.WebGLState:",q)}}function ye(){try{s.texSubImage2D.apply(s,arguments)}catch(q){console.error("THREE.WebGLState:",q)}}function Qe(){try{s.texSubImage3D.apply(s,arguments)}catch(q){console.error("THREE.WebGLState:",q)}}function Fe(){try{s.compressedTexSubImage2D.apply(s,arguments)}catch(q){console.error("THREE.WebGLState:",q)}}function He(){try{s.compressedTexSubImage3D.apply(s,arguments)}catch(q){console.error("THREE.WebGLState:",q)}}function ut(){try{s.texStorage2D.apply(s,arguments)}catch(q){console.error("THREE.WebGLState:",q)}}function Re(){try{s.texStorage3D.apply(s,arguments)}catch(q){console.error("THREE.WebGLState:",q)}}function ke(){try{s.texImage2D.apply(s,arguments)}catch(q){console.error("THREE.WebGLState:",q)}}function _t(){try{s.texImage3D.apply(s,arguments)}catch(q){console.error("THREE.WebGLState:",q)}}function at(q){ie.equals(q)===!1&&(s.scissor(q.x,q.y,q.z,q.w),ie.copy(q))}function Xe(q){Ae.equals(q)===!1&&(s.viewport(q.x,q.y,q.z,q.w),Ae.copy(q))}function ct(q,Le){let me=h.get(Le);me===void 0&&(me=new WeakMap,h.set(Le,me));let ge=me.get(q);ge===void 0&&(ge=s.getUniformBlockIndex(Le,q.name),me.set(q,ge))}function ft(q,Le){const ge=h.get(Le).get(q);c.get(Le)!==ge&&(s.uniformBlockBinding(Le,ge,q.__bindingPointIndex),c.set(Le,ge))}function bt(){s.disable(s.BLEND),s.disable(s.CULL_FACE),s.disable(s.DEPTH_TEST),s.disable(s.POLYGON_OFFSET_FILL),s.disable(s.SCISSOR_TEST),s.disable(s.STENCIL_TEST),s.disable(s.SAMPLE_ALPHA_TO_COVERAGE),s.blendEquation(s.FUNC_ADD),s.blendFunc(s.ONE,s.ZERO),s.blendFuncSeparate(s.ONE,s.ZERO,s.ONE,s.ZERO),s.blendColor(0,0,0,0),s.colorMask(!0,!0,!0,!0),s.clearColor(0,0,0,0),s.depthMask(!0),s.depthFunc(s.LESS),s.clearDepth(1),s.stencilMask(4294967295),s.stencilFunc(s.ALWAYS,0,4294967295),s.stencilOp(s.KEEP,s.KEEP,s.KEEP),s.clearStencil(0),s.cullFace(s.BACK),s.frontFace(s.CCW),s.polygonOffset(0,0),s.activeTexture(s.TEXTURE0),s.bindFramebuffer(s.FRAMEBUFFER,null),s.bindFramebuffer(s.DRAW_FRAMEBUFFER,null),s.bindFramebuffer(s.READ_FRAMEBUFFER,null),s.useProgram(null),s.lineWidth(1),s.scissor(0,0,s.canvas.width,s.canvas.height),s.viewport(0,0,s.canvas.width,s.canvas.height),d={},G=null,he={},m={},_=new WeakMap,g=[],S=null,M=!1,E=null,y=null,x=null,b=null,T=null,P=null,H=null,D=new yt(0,0,0),N=0,z=!1,C=null,w=null,U=null,X=null,k=null,ie.set(0,0,s.canvas.width,s.canvas.height),Ae.set(0,0,s.canvas.width,s.canvas.height),o.reset(),a.reset(),u.reset()}return{buffers:{color:o,depth:a,stencil:u},enable:xe,disable:de,bindFramebuffer:ee,drawBuffers:we,useProgram:Ue,setBlending:Te,setMaterial:Se,setFlipSided:Oe,setCullFace:pe,setLineWidth:et,setPolygonOffset:Be,setScissorTest:We,activeTexture:F,bindTexture:R,unbindTexture:oe,compressedTexImage2D:ve,compressedTexImage3D:_e,texImage2D:ke,texImage3D:_t,updateUBOMapping:ct,uniformBlockBinding:ft,texStorage2D:ut,texStorage3D:Re,texSubImage2D:ye,texSubImage3D:Qe,compressedTexSubImage2D:Fe,compressedTexSubImage3D:He,scissor:at,viewport:Xe,reset:bt}}function I0(s,e,t,i){const o=_T(i);switch(t){case p_:return s*e;case g_:return s*e;case __:return s*e*2;case v_:return s*e/o.components*o.byteLength;case _d:return s*e/o.components*o.byteLength;case y_:return s*e*2/o.components*o.byteLength;case vd:return s*e*2/o.components*o.byteLength;case m_:return s*e*3/o.components*o.byteLength;case mi:return s*e*4/o.components*o.byteLength;case yd:return s*e*4/o.components*o.byteLength;case _u:case vu:return Math.floor((s+3)/4)*Math.floor((e+3)/4)*8;case yu:case xu:return Math.floor((s+3)/4)*Math.floor((e+3)/4)*16;case Nh:case Uh:return Math.max(s,16)*Math.max(e,8)/4;case Dh:case Ih:return Math.max(s,8)*Math.max(e,8)/2;case Fh:case Oh:return Math.floor((s+3)/4)*Math.floor((e+3)/4)*8;case Bh:return Math.floor((s+3)/4)*Math.floor((e+3)/4)*16;case zh:return Math.floor((s+3)/4)*Math.floor((e+3)/4)*16;case kh:return Math.floor((s+4)/5)*Math.floor((e+3)/4)*16;case Hh:return Math.floor((s+4)/5)*Math.floor((e+4)/5)*16;case Vh:return Math.floor((s+5)/6)*Math.floor((e+4)/5)*16;case Gh:return Math.floor((s+5)/6)*Math.floor((e+5)/6)*16;case Wh:return Math.floor((s+7)/8)*Math.floor((e+4)/5)*16;case Xh:return Math.floor((s+7)/8)*Math.floor((e+5)/6)*16;case jh:return Math.floor((s+7)/8)*Math.floor((e+7)/8)*16;case qh:return Math.floor((s+9)/10)*Math.floor((e+4)/5)*16;case Yh:return Math.floor((s+9)/10)*Math.floor((e+5)/6)*16;case Kh:return Math.floor((s+9)/10)*Math.floor((e+7)/8)*16;case $h:return Math.floor((s+9)/10)*Math.floor((e+9)/10)*16;case Zh:return Math.floor((s+11)/12)*Math.floor((e+9)/10)*16;case Jh:return Math.floor((s+11)/12)*Math.floor((e+11)/12)*16;case Su:case Qh:case ed:return Math.ceil(s/4)*Math.ceil(e/4)*16;case x_:case td:return Math.ceil(s/4)*Math.ceil(e/4)*8;case nd:case id:return Math.ceil(s/4)*Math.ceil(e/4)*16}throw new Error(`Unable to determine texture byte length for ${t} format.`)}function _T(s){switch(s){case Ki:case f_:return{byteLength:1,components:1};case Sa:case h_:case Aa:return{byteLength:2,components:1};case md:case gd:return{byteLength:2,components:4};case os:case pd:case Xi:return{byteLength:4,components:1};case d_:return{byteLength:4,components:3}}throw new Error(`Unknown texture type ${s}.`)}function vT(s,e,t,i,o,a,u){const c=e.has("WEBGL_multisampled_render_to_texture")?e.get("WEBGL_multisampled_render_to_texture"):null,h=typeof navigator>"u"?!1:/OculusBrowser/g.test(navigator.userAgent),d=new Pe,m=new WeakMap;let _;const g=new WeakMap;let S=!1;try{S=typeof OffscreenCanvas<"u"&&new OffscreenCanvas(1,1).getContext("2d")!==null}catch{}function M(F,R){return S?new OffscreenCanvas(F,R):Ru("canvas")}function E(F,R,oe){let ve=1;const _e=We(F);if((_e.width>oe||_e.height>oe)&&(ve=oe/Math.max(_e.width,_e.height)),ve<1)if(typeof HTMLImageElement<"u"&&F instanceof HTMLImageElement||typeof HTMLCanvasElement<"u"&&F instanceof HTMLCanvasElement||typeof ImageBitmap<"u"&&F instanceof ImageBitmap||typeof VideoFrame<"u"&&F instanceof VideoFrame){const ye=Math.floor(ve*_e.width),Qe=Math.floor(ve*_e.height);_===void 0&&(_=M(ye,Qe));const Fe=R?M(ye,Qe):_;return Fe.width=ye,Fe.height=Qe,Fe.getContext("2d").drawImage(F,0,0,ye,Qe),console.warn("THREE.WebGLRenderer: Texture has been resized from ("+_e.width+"x"+_e.height+") to ("+ye+"x"+Qe+")."),Fe}else return"data"in F&&console.warn("THREE.WebGLRenderer: Image in DataTexture is too big ("+_e.width+"x"+_e.height+")."),F;return F}function y(F){return F.generateMipmaps&&F.minFilter!==ri&&F.minFilter!==mn}function x(F){s.generateMipmap(F)}function b(F,R,oe,ve,_e=!1){if(F!==null){if(s[F]!==void 0)return s[F];console.warn("THREE.WebGLRenderer: Attempt to use non-existing WebGL internal format '"+F+"'")}let ye=R;if(R===s.RED&&(oe===s.FLOAT&&(ye=s.R32F),oe===s.HALF_FLOAT&&(ye=s.R16F),oe===s.UNSIGNED_BYTE&&(ye=s.R8)),R===s.RED_INTEGER&&(oe===s.UNSIGNED_BYTE&&(ye=s.R8UI),oe===s.UNSIGNED_SHORT&&(ye=s.R16UI),oe===s.UNSIGNED_INT&&(ye=s.R32UI),oe===s.BYTE&&(ye=s.R8I),oe===s.SHORT&&(ye=s.R16I),oe===s.INT&&(ye=s.R32I)),R===s.RG&&(oe===s.FLOAT&&(ye=s.RG32F),oe===s.HALF_FLOAT&&(ye=s.RG16F),oe===s.UNSIGNED_BYTE&&(ye=s.RG8)),R===s.RG_INTEGER&&(oe===s.UNSIGNED_BYTE&&(ye=s.RG8UI),oe===s.UNSIGNED_SHORT&&(ye=s.RG16UI),oe===s.UNSIGNED_INT&&(ye=s.RG32UI),oe===s.BYTE&&(ye=s.RG8I),oe===s.SHORT&&(ye=s.RG16I),oe===s.INT&&(ye=s.RG32I)),R===s.RGB&&oe===s.UNSIGNED_INT_5_9_9_9_REV&&(ye=s.RGB9_E5),R===s.RGBA){const Qe=_e?wu:Tt.getTransfer(ve);oe===s.FLOAT&&(ye=s.RGBA32F),oe===s.HALF_FLOAT&&(ye=s.RGBA16F),oe===s.UNSIGNED_BYTE&&(ye=Qe===Ft?s.SRGB8_ALPHA8:s.RGBA8),oe===s.UNSIGNED_SHORT_4_4_4_4&&(ye=s.RGBA4),oe===s.UNSIGNED_SHORT_5_5_5_1&&(ye=s.RGB5_A1)}return(ye===s.R16F||ye===s.R32F||ye===s.RG16F||ye===s.RG32F||ye===s.RGBA16F||ye===s.RGBA32F)&&e.get("EXT_color_buffer_float"),ye}function T(F,R){let oe;return F?R===null||R===os||R===So?oe=s.DEPTH24_STENCIL8:R===Xi?oe=s.DEPTH32F_STENCIL8:R===Sa&&(oe=s.DEPTH24_STENCIL8,console.warn("DepthTexture: 16 bit depth attachment is not supported with stencil. Using 24-bit attachment.")):R===null||R===os||R===So?oe=s.DEPTH_COMPONENT24:R===Xi?oe=s.DEPTH_COMPONENT32F:R===Sa&&(oe=s.DEPTH_COMPONENT16),oe}function P(F,R){return y(F)===!0||F.isFramebufferTexture&&F.minFilter!==ri&&F.minFilter!==mn?Math.log2(Math.max(R.width,R.height))+1:F.mipmaps!==void 0&&F.mipmaps.length>0?F.mipmaps.length:F.isCompressedTexture&&Array.isArray(F.image)?R.mipmaps.length:1}function H(F){const R=F.target;R.removeEventListener("dispose",H),N(R),R.isVideoTexture&&m.delete(R)}function D(F){const R=F.target;R.removeEventListener("dispose",D),C(R)}function N(F){const R=i.get(F);if(R.__webglInit===void 0)return;const oe=F.source,ve=g.get(oe);if(ve){const _e=ve[R.__cacheKey];_e.usedTimes--,_e.usedTimes===0&&z(F),Object.keys(ve).length===0&&g.delete(oe)}i.remove(F)}function z(F){const R=i.get(F);s.deleteTexture(R.__webglTexture);const oe=F.source,ve=g.get(oe);delete ve[R.__cacheKey],u.memory.textures--}function C(F){const R=i.get(F);if(F.depthTexture&&F.depthTexture.dispose(),F.isWebGLCubeRenderTarget)for(let ve=0;ve<6;ve++){if(Array.isArray(R.__webglFramebuffer[ve]))for(let _e=0;_e<R.__webglFramebuffer[ve].length;_e++)s.deleteFramebuffer(R.__webglFramebuffer[ve][_e]);else s.deleteFramebuffer(R.__webglFramebuffer[ve]);R.__webglDepthbuffer&&s.deleteRenderbuffer(R.__webglDepthbuffer[ve])}else{if(Array.isArray(R.__webglFramebuffer))for(let ve=0;ve<R.__webglFramebuffer.length;ve++)s.deleteFramebuffer(R.__webglFramebuffer[ve]);else s.deleteFramebuffer(R.__webglFramebuffer);if(R.__webglDepthbuffer&&s.deleteRenderbuffer(R.__webglDepthbuffer),R.__webglMultisampledFramebuffer&&s.deleteFramebuffer(R.__webglMultisampledFramebuffer),R.__webglColorRenderbuffer)for(let ve=0;ve<R.__webglColorRenderbuffer.length;ve++)R.__webglColorRenderbuffer[ve]&&s.deleteRenderbuffer(R.__webglColorRenderbuffer[ve]);R.__webglDepthRenderbuffer&&s.deleteRenderbuffer(R.__webglDepthRenderbuffer)}const oe=F.textures;for(let ve=0,_e=oe.length;ve<_e;ve++){const ye=i.get(oe[ve]);ye.__webglTexture&&(s.deleteTexture(ye.__webglTexture),u.memory.textures--),i.remove(oe[ve])}i.remove(F)}let w=0;function U(){w=0}function X(){const F=w;return F>=o.maxTextures&&console.warn("THREE.WebGLTextures: Trying to use "+F+" texture units while this GPU supports only "+o.maxTextures),w+=1,F}function k(F){const R=[];return R.push(F.wrapS),R.push(F.wrapT),R.push(F.wrapR||0),R.push(F.magFilter),R.push(F.minFilter),R.push(F.anisotropy),R.push(F.internalFormat),R.push(F.format),R.push(F.type),R.push(F.generateMipmaps),R.push(F.premultiplyAlpha),R.push(F.flipY),R.push(F.unpackAlignment),R.push(F.colorSpace),R.join()}function Y(F,R){const oe=i.get(F);if(F.isVideoTexture&&et(F),F.isRenderTargetTexture===!1&&F.version>0&&oe.__version!==F.version){const ve=F.image;if(ve===null)console.warn("THREE.WebGLRenderer: Texture marked for update but no image data found.");else if(ve.complete===!1)console.warn("THREE.WebGLRenderer: Texture marked for update but image is incomplete");else{Ae(oe,F,R);return}}t.bindTexture(s.TEXTURE_2D,oe.__webglTexture,s.TEXTURE0+R)}function ae(F,R){const oe=i.get(F);if(F.version>0&&oe.__version!==F.version){Ae(oe,F,R);return}t.bindTexture(s.TEXTURE_2D_ARRAY,oe.__webglTexture,s.TEXTURE0+R)}function te(F,R){const oe=i.get(F);if(F.version>0&&oe.__version!==F.version){Ae(oe,F,R);return}t.bindTexture(s.TEXTURE_3D,oe.__webglTexture,s.TEXTURE0+R)}function ce(F,R){const oe=i.get(F);if(F.version>0&&oe.__version!==F.version){Z(oe,F,R);return}t.bindTexture(s.TEXTURE_CUBE_MAP,oe.__webglTexture,s.TEXTURE0+R)}const G={[bh]:s.REPEAT,[rs]:s.CLAMP_TO_EDGE,[Lh]:s.MIRRORED_REPEAT},he={[ri]:s.NEAREST,[oS]:s.NEAREST_MIPMAP_NEAREST,[Fl]:s.NEAREST_MIPMAP_LINEAR,[mn]:s.LINEAR,[Xf]:s.LINEAR_MIPMAP_NEAREST,[ss]:s.LINEAR_MIPMAP_LINEAR},re={[cS]:s.NEVER,[gS]:s.ALWAYS,[fS]:s.LESS,[M_]:s.LEQUAL,[hS]:s.EQUAL,[mS]:s.GEQUAL,[dS]:s.GREATER,[pS]:s.NOTEQUAL};function O(F,R){if(R.type===Xi&&e.has("OES_texture_float_linear")===!1&&(R.magFilter===mn||R.magFilter===Xf||R.magFilter===Fl||R.magFilter===ss||R.minFilter===mn||R.minFilter===Xf||R.minFilter===Fl||R.minFilter===ss)&&console.warn("THREE.WebGLRenderer: Unable to use linear filtering with floating point textures. OES_texture_float_linear not supported on this device."),s.texParameteri(F,s.TEXTURE_WRAP_S,G[R.wrapS]),s.texParameteri(F,s.TEXTURE_WRAP_T,G[R.wrapT]),(F===s.TEXTURE_3D||F===s.TEXTURE_2D_ARRAY)&&s.texParameteri(F,s.TEXTURE_WRAP_R,G[R.wrapR]),s.texParameteri(F,s.TEXTURE_MAG_FILTER,he[R.magFilter]),s.texParameteri(F,s.TEXTURE_MIN_FILTER,he[R.minFilter]),R.compareFunction&&(s.texParameteri(F,s.TEXTURE_COMPARE_MODE,s.COMPARE_REF_TO_TEXTURE),s.texParameteri(F,s.TEXTURE_COMPARE_FUNC,re[R.compareFunction])),e.has("EXT_texture_filter_anisotropic")===!0){if(R.magFilter===ri||R.minFilter!==Fl&&R.minFilter!==ss||R.type===Xi&&e.has("OES_texture_float_linear")===!1)return;if(R.anisotropy>1||i.get(R).__currentAnisotropy){const oe=e.get("EXT_texture_filter_anisotropic");s.texParameterf(F,oe.TEXTURE_MAX_ANISOTROPY_EXT,Math.min(R.anisotropy,o.getMaxAnisotropy())),i.get(R).__currentAnisotropy=R.anisotropy}}}function ie(F,R){let oe=!1;F.__webglInit===void 0&&(F.__webglInit=!0,R.addEventListener("dispose",H));const ve=R.source;let _e=g.get(ve);_e===void 0&&(_e={},g.set(ve,_e));const ye=k(R);if(ye!==F.__cacheKey){_e[ye]===void 0&&(_e[ye]={texture:s.createTexture(),usedTimes:0},u.memory.textures++,oe=!0),_e[ye].usedTimes++;const Qe=_e[F.__cacheKey];Qe!==void 0&&(_e[F.__cacheKey].usedTimes--,Qe.usedTimes===0&&z(R)),F.__cacheKey=ye,F.__webglTexture=_e[ye].texture}return oe}function Ae(F,R,oe){let ve=s.TEXTURE_2D;(R.isDataArrayTexture||R.isCompressedArrayTexture)&&(ve=s.TEXTURE_2D_ARRAY),R.isData3DTexture&&(ve=s.TEXTURE_3D);const _e=ie(F,R),ye=R.source;t.bindTexture(ve,F.__webglTexture,s.TEXTURE0+oe);const Qe=i.get(ye);if(ye.version!==Qe.__version||_e===!0){t.activeTexture(s.TEXTURE0+oe);const Fe=Tt.getPrimaries(Tt.workingColorSpace),He=R.colorSpace===Rr?null:Tt.getPrimaries(R.colorSpace),ut=R.colorSpace===Rr||Fe===He?s.NONE:s.BROWSER_DEFAULT_WEBGL;s.pixelStorei(s.UNPACK_FLIP_Y_WEBGL,R.flipY),s.pixelStorei(s.UNPACK_PREMULTIPLY_ALPHA_WEBGL,R.premultiplyAlpha),s.pixelStorei(s.UNPACK_ALIGNMENT,R.unpackAlignment),s.pixelStorei(s.UNPACK_COLORSPACE_CONVERSION_WEBGL,ut);let Re=E(R.image,!1,o.maxTextureSize);Re=Be(R,Re);const ke=a.convert(R.format,R.colorSpace),_t=a.convert(R.type);let at=b(R.internalFormat,ke,_t,R.colorSpace,R.isVideoTexture);O(ve,R);let Xe;const ct=R.mipmaps,ft=R.isVideoTexture!==!0,bt=Qe.__version===void 0||_e===!0,q=ye.dataReady,Le=P(R,Re);if(R.isDepthTexture)at=T(R.format===Mo,R.type),bt&&(ft?t.texStorage2D(s.TEXTURE_2D,1,at,Re.width,Re.height):t.texImage2D(s.TEXTURE_2D,0,at,Re.width,Re.height,0,ke,_t,null));else if(R.isDataTexture)if(ct.length>0){ft&&bt&&t.texStorage2D(s.TEXTURE_2D,Le,at,ct[0].width,ct[0].height);for(let me=0,ge=ct.length;me<ge;me++)Xe=ct[me],ft?q&&t.texSubImage2D(s.TEXTURE_2D,me,0,0,Xe.width,Xe.height,ke,_t,Xe.data):t.texImage2D(s.TEXTURE_2D,me,at,Xe.width,Xe.height,0,ke,_t,Xe.data);R.generateMipmaps=!1}else ft?(bt&&t.texStorage2D(s.TEXTURE_2D,Le,at,Re.width,Re.height),q&&t.texSubImage2D(s.TEXTURE_2D,0,0,0,Re.width,Re.height,ke,_t,Re.data)):t.texImage2D(s.TEXTURE_2D,0,at,Re.width,Re.height,0,ke,_t,Re.data);else if(R.isCompressedTexture)if(R.isCompressedArrayTexture){ft&&bt&&t.texStorage3D(s.TEXTURE_2D_ARRAY,Le,at,ct[0].width,ct[0].height,Re.depth);for(let me=0,ge=ct.length;me<ge;me++)if(Xe=ct[me],R.format!==mi)if(ke!==null)if(ft){if(q)if(R.layerUpdates.size>0){const Ne=I0(Xe.width,Xe.height,R.format,R.type);for(const rt of R.layerUpdates){const vt=Xe.data.subarray(rt*Ne/Xe.data.BYTES_PER_ELEMENT,(rt+1)*Ne/Xe.data.BYTES_PER_ELEMENT);t.compressedTexSubImage3D(s.TEXTURE_2D_ARRAY,me,0,0,rt,Xe.width,Xe.height,1,ke,vt,0,0)}R.clearLayerUpdates()}else t.compressedTexSubImage3D(s.TEXTURE_2D_ARRAY,me,0,0,0,Xe.width,Xe.height,Re.depth,ke,Xe.data,0,0)}else t.compressedTexImage3D(s.TEXTURE_2D_ARRAY,me,at,Xe.width,Xe.height,Re.depth,0,Xe.data,0,0);else console.warn("THREE.WebGLRenderer: Attempt to load unsupported compressed texture format in .uploadTexture()");else ft?q&&t.texSubImage3D(s.TEXTURE_2D_ARRAY,me,0,0,0,Xe.width,Xe.height,Re.depth,ke,_t,Xe.data):t.texImage3D(s.TEXTURE_2D_ARRAY,me,at,Xe.width,Xe.height,Re.depth,0,ke,_t,Xe.data)}else{ft&&bt&&t.texStorage2D(s.TEXTURE_2D,Le,at,ct[0].width,ct[0].height);for(let me=0,ge=ct.length;me<ge;me++)Xe=ct[me],R.format!==mi?ke!==null?ft?q&&t.compressedTexSubImage2D(s.TEXTURE_2D,me,0,0,Xe.width,Xe.height,ke,Xe.data):t.compressedTexImage2D(s.TEXTURE_2D,me,at,Xe.width,Xe.height,0,Xe.data):console.warn("THREE.WebGLRenderer: Attempt to load unsupported compressed texture format in .uploadTexture()"):ft?q&&t.texSubImage2D(s.TEXTURE_2D,me,0,0,Xe.width,Xe.height,ke,_t,Xe.data):t.texImage2D(s.TEXTURE_2D,me,at,Xe.width,Xe.height,0,ke,_t,Xe.data)}else if(R.isDataArrayTexture)if(ft){if(bt&&t.texStorage3D(s.TEXTURE_2D_ARRAY,Le,at,Re.width,Re.height,Re.depth),q)if(R.layerUpdates.size>0){const me=I0(Re.width,Re.height,R.format,R.type);for(const ge of R.layerUpdates){const Ne=Re.data.subarray(ge*me/Re.data.BYTES_PER_ELEMENT,(ge+1)*me/Re.data.BYTES_PER_ELEMENT);t.texSubImage3D(s.TEXTURE_2D_ARRAY,0,0,0,ge,Re.width,Re.height,1,ke,_t,Ne)}R.clearLayerUpdates()}else t.texSubImage3D(s.TEXTURE_2D_ARRAY,0,0,0,0,Re.width,Re.height,Re.depth,ke,_t,Re.data)}else t.texImage3D(s.TEXTURE_2D_ARRAY,0,at,Re.width,Re.height,Re.depth,0,ke,_t,Re.data);else if(R.isData3DTexture)ft?(bt&&t.texStorage3D(s.TEXTURE_3D,Le,at,Re.width,Re.height,Re.depth),q&&t.texSubImage3D(s.TEXTURE_3D,0,0,0,0,Re.width,Re.height,Re.depth,ke,_t,Re.data)):t.texImage3D(s.TEXTURE_3D,0,at,Re.width,Re.height,Re.depth,0,ke,_t,Re.data);else if(R.isFramebufferTexture){if(bt)if(ft)t.texStorage2D(s.TEXTURE_2D,Le,at,Re.width,Re.height);else{let me=Re.width,ge=Re.height;for(let Ne=0;Ne<Le;Ne++)t.texImage2D(s.TEXTURE_2D,Ne,at,me,ge,0,ke,_t,null),me>>=1,ge>>=1}}else if(ct.length>0){if(ft&&bt){const me=We(ct[0]);t.texStorage2D(s.TEXTURE_2D,Le,at,me.width,me.height)}for(let me=0,ge=ct.length;me<ge;me++)Xe=ct[me],ft?q&&t.texSubImage2D(s.TEXTURE_2D,me,0,0,ke,_t,Xe):t.texImage2D(s.TEXTURE_2D,me,at,ke,_t,Xe);R.generateMipmaps=!1}else if(ft){if(bt){const me=We(Re);t.texStorage2D(s.TEXTURE_2D,Le,at,me.width,me.height)}q&&t.texSubImage2D(s.TEXTURE_2D,0,0,0,ke,_t,Re)}else t.texImage2D(s.TEXTURE_2D,0,at,ke,_t,Re);y(R)&&x(ve),Qe.__version=ye.version,R.onUpdate&&R.onUpdate(R)}F.__version=R.version}function Z(F,R,oe){if(R.image.length!==6)return;const ve=ie(F,R),_e=R.source;t.bindTexture(s.TEXTURE_CUBE_MAP,F.__webglTexture,s.TEXTURE0+oe);const ye=i.get(_e);if(_e.version!==ye.__version||ve===!0){t.activeTexture(s.TEXTURE0+oe);const Qe=Tt.getPrimaries(Tt.workingColorSpace),Fe=R.colorSpace===Rr?null:Tt.getPrimaries(R.colorSpace),He=R.colorSpace===Rr||Qe===Fe?s.NONE:s.BROWSER_DEFAULT_WEBGL;s.pixelStorei(s.UNPACK_FLIP_Y_WEBGL,R.flipY),s.pixelStorei(s.UNPACK_PREMULTIPLY_ALPHA_WEBGL,R.premultiplyAlpha),s.pixelStorei(s.UNPACK_ALIGNMENT,R.unpackAlignment),s.pixelStorei(s.UNPACK_COLORSPACE_CONVERSION_WEBGL,He);const ut=R.isCompressedTexture||R.image[0].isCompressedTexture,Re=R.image[0]&&R.image[0].isDataTexture,ke=[];for(let ge=0;ge<6;ge++)!ut&&!Re?ke[ge]=E(R.image[ge],!0,o.maxCubemapSize):ke[ge]=Re?R.image[ge].image:R.image[ge],ke[ge]=Be(R,ke[ge]);const _t=ke[0],at=a.convert(R.format,R.colorSpace),Xe=a.convert(R.type),ct=b(R.internalFormat,at,Xe,R.colorSpace),ft=R.isVideoTexture!==!0,bt=ye.__version===void 0||ve===!0,q=_e.dataReady;let Le=P(R,_t);O(s.TEXTURE_CUBE_MAP,R);let me;if(ut){ft&&bt&&t.texStorage2D(s.TEXTURE_CUBE_MAP,Le,ct,_t.width,_t.height);for(let ge=0;ge<6;ge++){me=ke[ge].mipmaps;for(let Ne=0;Ne<me.length;Ne++){const rt=me[Ne];R.format!==mi?at!==null?ft?q&&t.compressedTexSubImage2D(s.TEXTURE_CUBE_MAP_POSITIVE_X+ge,Ne,0,0,rt.width,rt.height,at,rt.data):t.compressedTexImage2D(s.TEXTURE_CUBE_MAP_POSITIVE_X+ge,Ne,ct,rt.width,rt.height,0,rt.data):console.warn("THREE.WebGLRenderer: Attempt to load unsupported compressed texture format in .setTextureCube()"):ft?q&&t.texSubImage2D(s.TEXTURE_CUBE_MAP_POSITIVE_X+ge,Ne,0,0,rt.width,rt.height,at,Xe,rt.data):t.texImage2D(s.TEXTURE_CUBE_MAP_POSITIVE_X+ge,Ne,ct,rt.width,rt.height,0,at,Xe,rt.data)}}}else{if(me=R.mipmaps,ft&&bt){me.length>0&&Le++;const ge=We(ke[0]);t.texStorage2D(s.TEXTURE_CUBE_MAP,Le,ct,ge.width,ge.height)}for(let ge=0;ge<6;ge++)if(Re){ft?q&&t.texSubImage2D(s.TEXTURE_CUBE_MAP_POSITIVE_X+ge,0,0,0,ke[ge].width,ke[ge].height,at,Xe,ke[ge].data):t.texImage2D(s.TEXTURE_CUBE_MAP_POSITIVE_X+ge,0,ct,ke[ge].width,ke[ge].height,0,at,Xe,ke[ge].data);for(let Ne=0;Ne<me.length;Ne++){const vt=me[Ne].image[ge].image;ft?q&&t.texSubImage2D(s.TEXTURE_CUBE_MAP_POSITIVE_X+ge,Ne+1,0,0,vt.width,vt.height,at,Xe,vt.data):t.texImage2D(s.TEXTURE_CUBE_MAP_POSITIVE_X+ge,Ne+1,ct,vt.width,vt.height,0,at,Xe,vt.data)}}else{ft?q&&t.texSubImage2D(s.TEXTURE_CUBE_MAP_POSITIVE_X+ge,0,0,0,at,Xe,ke[ge]):t.texImage2D(s.TEXTURE_CUBE_MAP_POSITIVE_X+ge,0,ct,at,Xe,ke[ge]);for(let Ne=0;Ne<me.length;Ne++){const rt=me[Ne];ft?q&&t.texSubImage2D(s.TEXTURE_CUBE_MAP_POSITIVE_X+ge,Ne+1,0,0,at,Xe,rt.image[ge]):t.texImage2D(s.TEXTURE_CUBE_MAP_POSITIVE_X+ge,Ne+1,ct,at,Xe,rt.image[ge])}}}y(R)&&x(s.TEXTURE_CUBE_MAP),ye.__version=_e.version,R.onUpdate&&R.onUpdate(R)}F.__version=R.version}function ne(F,R,oe,ve,_e,ye){const Qe=a.convert(oe.format,oe.colorSpace),Fe=a.convert(oe.type),He=b(oe.internalFormat,Qe,Fe,oe.colorSpace);if(!i.get(R).__hasExternalTextures){const Re=Math.max(1,R.width>>ye),ke=Math.max(1,R.height>>ye);_e===s.TEXTURE_3D||_e===s.TEXTURE_2D_ARRAY?t.texImage3D(_e,ye,He,Re,ke,R.depth,0,Qe,Fe,null):t.texImage2D(_e,ye,He,Re,ke,0,Qe,Fe,null)}t.bindFramebuffer(s.FRAMEBUFFER,F),pe(R)?c.framebufferTexture2DMultisampleEXT(s.FRAMEBUFFER,ve,_e,i.get(oe).__webglTexture,0,Oe(R)):(_e===s.TEXTURE_2D||_e>=s.TEXTURE_CUBE_MAP_POSITIVE_X&&_e<=s.TEXTURE_CUBE_MAP_NEGATIVE_Z)&&s.framebufferTexture2D(s.FRAMEBUFFER,ve,_e,i.get(oe).__webglTexture,ye),t.bindFramebuffer(s.FRAMEBUFFER,null)}function xe(F,R,oe){if(s.bindRenderbuffer(s.RENDERBUFFER,F),R.depthBuffer){const ve=R.depthTexture,_e=ve&&ve.isDepthTexture?ve.type:null,ye=T(R.stencilBuffer,_e),Qe=R.stencilBuffer?s.DEPTH_STENCIL_ATTACHMENT:s.DEPTH_ATTACHMENT,Fe=Oe(R);pe(R)?c.renderbufferStorageMultisampleEXT(s.RENDERBUFFER,Fe,ye,R.width,R.height):oe?s.renderbufferStorageMultisample(s.RENDERBUFFER,Fe,ye,R.width,R.height):s.renderbufferStorage(s.RENDERBUFFER,ye,R.width,R.height),s.framebufferRenderbuffer(s.FRAMEBUFFER,Qe,s.RENDERBUFFER,F)}else{const ve=R.textures;for(let _e=0;_e<ve.length;_e++){const ye=ve[_e],Qe=a.convert(ye.format,ye.colorSpace),Fe=a.convert(ye.type),He=b(ye.internalFormat,Qe,Fe,ye.colorSpace),ut=Oe(R);oe&&pe(R)===!1?s.renderbufferStorageMultisample(s.RENDERBUFFER,ut,He,R.width,R.height):pe(R)?c.renderbufferStorageMultisampleEXT(s.RENDERBUFFER,ut,He,R.width,R.height):s.renderbufferStorage(s.RENDERBUFFER,He,R.width,R.height)}}s.bindRenderbuffer(s.RENDERBUFFER,null)}function de(F,R){if(R&&R.isWebGLCubeRenderTarget)throw new Error("Depth Texture with cube render targets is not supported");if(t.bindFramebuffer(s.FRAMEBUFFER,F),!(R.depthTexture&&R.depthTexture.isDepthTexture))throw new Error("renderTarget.depthTexture must be an instance of THREE.DepthTexture");(!i.get(R.depthTexture).__webglTexture||R.depthTexture.image.width!==R.width||R.depthTexture.image.height!==R.height)&&(R.depthTexture.image.width=R.width,R.depthTexture.image.height=R.height,R.depthTexture.needsUpdate=!0),Y(R.depthTexture,0);const ve=i.get(R.depthTexture).__webglTexture,_e=Oe(R);if(R.depthTexture.format===mo)pe(R)?c.framebufferTexture2DMultisampleEXT(s.FRAMEBUFFER,s.DEPTH_ATTACHMENT,s.TEXTURE_2D,ve,0,_e):s.framebufferTexture2D(s.FRAMEBUFFER,s.DEPTH_ATTACHMENT,s.TEXTURE_2D,ve,0);else if(R.depthTexture.format===Mo)pe(R)?c.framebufferTexture2DMultisampleEXT(s.FRAMEBUFFER,s.DEPTH_STENCIL_ATTACHMENT,s.TEXTURE_2D,ve,0,_e):s.framebufferTexture2D(s.FRAMEBUFFER,s.DEPTH_STENCIL_ATTACHMENT,s.TEXTURE_2D,ve,0);else throw new Error("Unknown depthTexture format")}function ee(F){const R=i.get(F),oe=F.isWebGLCubeRenderTarget===!0;if(R.__boundDepthTexture!==F.depthTexture){const ve=F.depthTexture;if(R.__depthDisposeCallback&&R.__depthDisposeCallback(),ve){const _e=()=>{delete R.__boundDepthTexture,delete R.__depthDisposeCallback,ve.removeEventListener("dispose",_e)};ve.addEventListener("dispose",_e),R.__depthDisposeCallback=_e}R.__boundDepthTexture=ve}if(F.depthTexture&&!R.__autoAllocateDepthBuffer){if(oe)throw new Error("target.depthTexture not supported in Cube render targets");de(R.__webglFramebuffer,F)}else if(oe){R.__webglDepthbuffer=[];for(let ve=0;ve<6;ve++)if(t.bindFramebuffer(s.FRAMEBUFFER,R.__webglFramebuffer[ve]),R.__webglDepthbuffer[ve]===void 0)R.__webglDepthbuffer[ve]=s.createRenderbuffer(),xe(R.__webglDepthbuffer[ve],F,!1);else{const _e=F.stencilBuffer?s.DEPTH_STENCIL_ATTACHMENT:s.DEPTH_ATTACHMENT,ye=R.__webglDepthbuffer[ve];s.bindRenderbuffer(s.RENDERBUFFER,ye),s.framebufferRenderbuffer(s.FRAMEBUFFER,_e,s.RENDERBUFFER,ye)}}else if(t.bindFramebuffer(s.FRAMEBUFFER,R.__webglFramebuffer),R.__webglDepthbuffer===void 0)R.__webglDepthbuffer=s.createRenderbuffer(),xe(R.__webglDepthbuffer,F,!1);else{const ve=F.stencilBuffer?s.DEPTH_STENCIL_ATTACHMENT:s.DEPTH_ATTACHMENT,_e=R.__webglDepthbuffer;s.bindRenderbuffer(s.RENDERBUFFER,_e),s.framebufferRenderbuffer(s.FRAMEBUFFER,ve,s.RENDERBUFFER,_e)}t.bindFramebuffer(s.FRAMEBUFFER,null)}function we(F,R,oe){const ve=i.get(F);R!==void 0&&ne(ve.__webglFramebuffer,F,F.texture,s.COLOR_ATTACHMENT0,s.TEXTURE_2D,0),oe!==void 0&&ee(F)}function Ue(F){const R=F.texture,oe=i.get(F),ve=i.get(R);F.addEventListener("dispose",D);const _e=F.textures,ye=F.isWebGLCubeRenderTarget===!0,Qe=_e.length>1;if(Qe||(ve.__webglTexture===void 0&&(ve.__webglTexture=s.createTexture()),ve.__version=R.version,u.memory.textures++),ye){oe.__webglFramebuffer=[];for(let Fe=0;Fe<6;Fe++)if(R.mipmaps&&R.mipmaps.length>0){oe.__webglFramebuffer[Fe]=[];for(let He=0;He<R.mipmaps.length;He++)oe.__webglFramebuffer[Fe][He]=s.createFramebuffer()}else oe.__webglFramebuffer[Fe]=s.createFramebuffer()}else{if(R.mipmaps&&R.mipmaps.length>0){oe.__webglFramebuffer=[];for(let Fe=0;Fe<R.mipmaps.length;Fe++)oe.__webglFramebuffer[Fe]=s.createFramebuffer()}else oe.__webglFramebuffer=s.createFramebuffer();if(Qe)for(let Fe=0,He=_e.length;Fe<He;Fe++){const ut=i.get(_e[Fe]);ut.__webglTexture===void 0&&(ut.__webglTexture=s.createTexture(),u.memory.textures++)}if(F.samples>0&&pe(F)===!1){oe.__webglMultisampledFramebuffer=s.createFramebuffer(),oe.__webglColorRenderbuffer=[],t.bindFramebuffer(s.FRAMEBUFFER,oe.__webglMultisampledFramebuffer);for(let Fe=0;Fe<_e.length;Fe++){const He=_e[Fe];oe.__webglColorRenderbuffer[Fe]=s.createRenderbuffer(),s.bindRenderbuffer(s.RENDERBUFFER,oe.__webglColorRenderbuffer[Fe]);const ut=a.convert(He.format,He.colorSpace),Re=a.convert(He.type),ke=b(He.internalFormat,ut,Re,He.colorSpace,F.isXRRenderTarget===!0),_t=Oe(F);s.renderbufferStorageMultisample(s.RENDERBUFFER,_t,ke,F.width,F.height),s.framebufferRenderbuffer(s.FRAMEBUFFER,s.COLOR_ATTACHMENT0+Fe,s.RENDERBUFFER,oe.__webglColorRenderbuffer[Fe])}s.bindRenderbuffer(s.RENDERBUFFER,null),F.depthBuffer&&(oe.__webglDepthRenderbuffer=s.createRenderbuffer(),xe(oe.__webglDepthRenderbuffer,F,!0)),t.bindFramebuffer(s.FRAMEBUFFER,null)}}if(ye){t.bindTexture(s.TEXTURE_CUBE_MAP,ve.__webglTexture),O(s.TEXTURE_CUBE_MAP,R);for(let Fe=0;Fe<6;Fe++)if(R.mipmaps&&R.mipmaps.length>0)for(let He=0;He<R.mipmaps.length;He++)ne(oe.__webglFramebuffer[Fe][He],F,R,s.COLOR_ATTACHMENT0,s.TEXTURE_CUBE_MAP_POSITIVE_X+Fe,He);else ne(oe.__webglFramebuffer[Fe],F,R,s.COLOR_ATTACHMENT0,s.TEXTURE_CUBE_MAP_POSITIVE_X+Fe,0);y(R)&&x(s.TEXTURE_CUBE_MAP),t.unbindTexture()}else if(Qe){for(let Fe=0,He=_e.length;Fe<He;Fe++){const ut=_e[Fe],Re=i.get(ut);t.bindTexture(s.TEXTURE_2D,Re.__webglTexture),O(s.TEXTURE_2D,ut),ne(oe.__webglFramebuffer,F,ut,s.COLOR_ATTACHMENT0+Fe,s.TEXTURE_2D,0),y(ut)&&x(s.TEXTURE_2D)}t.unbindTexture()}else{let Fe=s.TEXTURE_2D;if((F.isWebGL3DRenderTarget||F.isWebGLArrayRenderTarget)&&(Fe=F.isWebGL3DRenderTarget?s.TEXTURE_3D:s.TEXTURE_2D_ARRAY),t.bindTexture(Fe,ve.__webglTexture),O(Fe,R),R.mipmaps&&R.mipmaps.length>0)for(let He=0;He<R.mipmaps.length;He++)ne(oe.__webglFramebuffer[He],F,R,s.COLOR_ATTACHMENT0,Fe,He);else ne(oe.__webglFramebuffer,F,R,s.COLOR_ATTACHMENT0,Fe,0);y(R)&&x(Fe),t.unbindTexture()}F.depthBuffer&&ee(F)}function tt(F){const R=F.textures;for(let oe=0,ve=R.length;oe<ve;oe++){const _e=R[oe];if(y(_e)){const ye=F.isWebGLCubeRenderTarget?s.TEXTURE_CUBE_MAP:s.TEXTURE_2D,Qe=i.get(_e).__webglTexture;t.bindTexture(ye,Qe),x(ye),t.unbindTexture()}}}const I=[],Te=[];function Se(F){if(F.samples>0){if(pe(F)===!1){const R=F.textures,oe=F.width,ve=F.height;let _e=s.COLOR_BUFFER_BIT;const ye=F.stencilBuffer?s.DEPTH_STENCIL_ATTACHMENT:s.DEPTH_ATTACHMENT,Qe=i.get(F),Fe=R.length>1;if(Fe)for(let He=0;He<R.length;He++)t.bindFramebuffer(s.FRAMEBUFFER,Qe.__webglMultisampledFramebuffer),s.framebufferRenderbuffer(s.FRAMEBUFFER,s.COLOR_ATTACHMENT0+He,s.RENDERBUFFER,null),t.bindFramebuffer(s.FRAMEBUFFER,Qe.__webglFramebuffer),s.framebufferTexture2D(s.DRAW_FRAMEBUFFER,s.COLOR_ATTACHMENT0+He,s.TEXTURE_2D,null,0);t.bindFramebuffer(s.READ_FRAMEBUFFER,Qe.__webglMultisampledFramebuffer),t.bindFramebuffer(s.DRAW_FRAMEBUFFER,Qe.__webglFramebuffer);for(let He=0;He<R.length;He++){if(F.resolveDepthBuffer&&(F.depthBuffer&&(_e|=s.DEPTH_BUFFER_BIT),F.stencilBuffer&&F.resolveStencilBuffer&&(_e|=s.STENCIL_BUFFER_BIT)),Fe){s.framebufferRenderbuffer(s.READ_FRAMEBUFFER,s.COLOR_ATTACHMENT0,s.RENDERBUFFER,Qe.__webglColorRenderbuffer[He]);const ut=i.get(R[He]).__webglTexture;s.framebufferTexture2D(s.DRAW_FRAMEBUFFER,s.COLOR_ATTACHMENT0,s.TEXTURE_2D,ut,0)}s.blitFramebuffer(0,0,oe,ve,0,0,oe,ve,_e,s.NEAREST),h===!0&&(I.length=0,Te.length=0,I.push(s.COLOR_ATTACHMENT0+He),F.depthBuffer&&F.resolveDepthBuffer===!1&&(I.push(ye),Te.push(ye),s.invalidateFramebuffer(s.DRAW_FRAMEBUFFER,Te)),s.invalidateFramebuffer(s.READ_FRAMEBUFFER,I))}if(t.bindFramebuffer(s.READ_FRAMEBUFFER,null),t.bindFramebuffer(s.DRAW_FRAMEBUFFER,null),Fe)for(let He=0;He<R.length;He++){t.bindFramebuffer(s.FRAMEBUFFER,Qe.__webglMultisampledFramebuffer),s.framebufferRenderbuffer(s.FRAMEBUFFER,s.COLOR_ATTACHMENT0+He,s.RENDERBUFFER,Qe.__webglColorRenderbuffer[He]);const ut=i.get(R[He]).__webglTexture;t.bindFramebuffer(s.FRAMEBUFFER,Qe.__webglFramebuffer),s.framebufferTexture2D(s.DRAW_FRAMEBUFFER,s.COLOR_ATTACHMENT0+He,s.TEXTURE_2D,ut,0)}t.bindFramebuffer(s.DRAW_FRAMEBUFFER,Qe.__webglMultisampledFramebuffer)}else if(F.depthBuffer&&F.resolveDepthBuffer===!1&&h){const R=F.stencilBuffer?s.DEPTH_STENCIL_ATTACHMENT:s.DEPTH_ATTACHMENT;s.invalidateFramebuffer(s.DRAW_FRAMEBUFFER,[R])}}}function Oe(F){return Math.min(o.maxSamples,F.samples)}function pe(F){const R=i.get(F);return F.samples>0&&e.has("WEBGL_multisampled_render_to_texture")===!0&&R.__useRenderToTexture!==!1}function et(F){const R=u.render.frame;m.get(F)!==R&&(m.set(F,R),F.update())}function Be(F,R){const oe=F.colorSpace,ve=F.format,_e=F.type;return F.isCompressedTexture===!0||F.isVideoTexture===!0||oe!==Ir&&oe!==Rr&&(Tt.getTransfer(oe)===Ft?(ve!==mi||_e!==Ki)&&console.warn("THREE.WebGLTextures: sRGB encoded textures have to use RGBAFormat and UnsignedByteType."):console.error("THREE.WebGLTextures: Unsupported texture color space:",oe)),R}function We(F){return typeof HTMLImageElement<"u"&&F instanceof HTMLImageElement?(d.width=F.naturalWidth||F.width,d.height=F.naturalHeight||F.height):typeof VideoFrame<"u"&&F instanceof VideoFrame?(d.width=F.displayWidth,d.height=F.displayHeight):(d.width=F.width,d.height=F.height),d}this.allocateTextureUnit=X,this.resetTextureUnits=U,this.setTexture2D=Y,this.setTexture2DArray=ae,this.setTexture3D=te,this.setTextureCube=ce,this.rebindTextures=we,this.setupRenderTarget=Ue,this.updateRenderTargetMipmap=tt,this.updateMultisampleRenderTarget=Se,this.setupDepthRenderbuffer=ee,this.setupFrameBufferTexture=ne,this.useMultisampledRTT=pe}function yT(s,e){function t(i,o=Rr){let a;const u=Tt.getTransfer(o);if(i===Ki)return s.UNSIGNED_BYTE;if(i===md)return s.UNSIGNED_SHORT_4_4_4_4;if(i===gd)return s.UNSIGNED_SHORT_5_5_5_1;if(i===d_)return s.UNSIGNED_INT_5_9_9_9_REV;if(i===f_)return s.BYTE;if(i===h_)return s.SHORT;if(i===Sa)return s.UNSIGNED_SHORT;if(i===pd)return s.INT;if(i===os)return s.UNSIGNED_INT;if(i===Xi)return s.FLOAT;if(i===Aa)return s.HALF_FLOAT;if(i===p_)return s.ALPHA;if(i===m_)return s.RGB;if(i===mi)return s.RGBA;if(i===g_)return s.LUMINANCE;if(i===__)return s.LUMINANCE_ALPHA;if(i===mo)return s.DEPTH_COMPONENT;if(i===Mo)return s.DEPTH_STENCIL;if(i===v_)return s.RED;if(i===_d)return s.RED_INTEGER;if(i===y_)return s.RG;if(i===vd)return s.RG_INTEGER;if(i===yd)return s.RGBA_INTEGER;if(i===_u||i===vu||i===yu||i===xu)if(u===Ft)if(a=e.get("WEBGL_compressed_texture_s3tc_srgb"),a!==null){if(i===_u)return a.COMPRESSED_SRGB_S3TC_DXT1_EXT;if(i===vu)return a.COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;if(i===yu)return a.COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;if(i===xu)return a.COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT}else return null;else if(a=e.get("WEBGL_compressed_texture_s3tc"),a!==null){if(i===_u)return a.COMPRESSED_RGB_S3TC_DXT1_EXT;if(i===vu)return a.COMPRESSED_RGBA_S3TC_DXT1_EXT;if(i===yu)return a.COMPRESSED_RGBA_S3TC_DXT3_EXT;if(i===xu)return a.COMPRESSED_RGBA_S3TC_DXT5_EXT}else return null;if(i===Dh||i===Nh||i===Ih||i===Uh)if(a=e.get("WEBGL_compressed_texture_pvrtc"),a!==null){if(i===Dh)return a.COMPRESSED_RGB_PVRTC_4BPPV1_IMG;if(i===Nh)return a.COMPRESSED_RGB_PVRTC_2BPPV1_IMG;if(i===Ih)return a.COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;if(i===Uh)return a.COMPRESSED_RGBA_PVRTC_2BPPV1_IMG}else return null;if(i===Fh||i===Oh||i===Bh)if(a=e.get("WEBGL_compressed_texture_etc"),a!==null){if(i===Fh||i===Oh)return u===Ft?a.COMPRESSED_SRGB8_ETC2:a.COMPRESSED_RGB8_ETC2;if(i===Bh)return u===Ft?a.COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:a.COMPRESSED_RGBA8_ETC2_EAC}else return null;if(i===zh||i===kh||i===Hh||i===Vh||i===Gh||i===Wh||i===Xh||i===jh||i===qh||i===Yh||i===Kh||i===$h||i===Zh||i===Jh)if(a=e.get("WEBGL_compressed_texture_astc"),a!==null){if(i===zh)return u===Ft?a.COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR:a.COMPRESSED_RGBA_ASTC_4x4_KHR;if(i===kh)return u===Ft?a.COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR:a.COMPRESSED_RGBA_ASTC_5x4_KHR;if(i===Hh)return u===Ft?a.COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR:a.COMPRESSED_RGBA_ASTC_5x5_KHR;if(i===Vh)return u===Ft?a.COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR:a.COMPRESSED_RGBA_ASTC_6x5_KHR;if(i===Gh)return u===Ft?a.COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR:a.COMPRESSED_RGBA_ASTC_6x6_KHR;if(i===Wh)return u===Ft?a.COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR:a.COMPRESSED_RGBA_ASTC_8x5_KHR;if(i===Xh)return u===Ft?a.COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR:a.COMPRESSED_RGBA_ASTC_8x6_KHR;if(i===jh)return u===Ft?a.COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR:a.COMPRESSED_RGBA_ASTC_8x8_KHR;if(i===qh)return u===Ft?a.COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR:a.COMPRESSED_RGBA_ASTC_10x5_KHR;if(i===Yh)return u===Ft?a.COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR:a.COMPRESSED_RGBA_ASTC_10x6_KHR;if(i===Kh)return u===Ft?a.COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR:a.COMPRESSED_RGBA_ASTC_10x8_KHR;if(i===$h)return u===Ft?a.COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR:a.COMPRESSED_RGBA_ASTC_10x10_KHR;if(i===Zh)return u===Ft?a.COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR:a.COMPRESSED_RGBA_ASTC_12x10_KHR;if(i===Jh)return u===Ft?a.COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR:a.COMPRESSED_RGBA_ASTC_12x12_KHR}else return null;if(i===Su||i===Qh||i===ed)if(a=e.get("EXT_texture_compression_bptc"),a!==null){if(i===Su)return u===Ft?a.COMPRESSED_SRGB_ALPHA_BPTC_UNORM_EXT:a.COMPRESSED_RGBA_BPTC_UNORM_EXT;if(i===Qh)return a.COMPRESSED_RGB_BPTC_SIGNED_FLOAT_EXT;if(i===ed)return a.COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_EXT}else return null;if(i===x_||i===td||i===nd||i===id)if(a=e.get("EXT_texture_compression_rgtc"),a!==null){if(i===Su)return a.COMPRESSED_RED_RGTC1_EXT;if(i===td)return a.COMPRESSED_SIGNED_RED_RGTC1_EXT;if(i===nd)return a.COMPRESSED_RED_GREEN_RGTC2_EXT;if(i===id)return a.COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT}else return null;return i===So?s.UNSIGNED_INT_24_8:s[i]!==void 0?s[i]:null}return{convert:t}}class xT extends ni{constructor(e=[]){super(),this.isArrayCamera=!0,this.cameras=e}}class qi extends cn{constructor(){super(),this.isGroup=!0,this.type="Group"}}const ST={type:"move"};class _h{constructor(){this._targetRay=null,this._grip=null,this._hand=null}getHandSpace(){return this._hand===null&&(this._hand=new qi,this._hand.matrixAutoUpdate=!1,this._hand.visible=!1,this._hand.joints={},this._hand.inputState={pinching:!1}),this._hand}getTargetRaySpace(){return this._targetRay===null&&(this._targetRay=new qi,this._targetRay.matrixAutoUpdate=!1,this._targetRay.visible=!1,this._targetRay.hasLinearVelocity=!1,this._targetRay.linearVelocity=new W,this._targetRay.hasAngularVelocity=!1,this._targetRay.angularVelocity=new W),this._targetRay}getGripSpace(){return this._grip===null&&(this._grip=new qi,this._grip.matrixAutoUpdate=!1,this._grip.visible=!1,this._grip.hasLinearVelocity=!1,this._grip.linearVelocity=new W,this._grip.hasAngularVelocity=!1,this._grip.angularVelocity=new W),this._grip}dispatchEvent(e){return this._targetRay!==null&&this._targetRay.dispatchEvent(e),this._grip!==null&&this._grip.dispatchEvent(e),this._hand!==null&&this._hand.dispatchEvent(e),this}connect(e){if(e&&e.hand){const t=this._hand;if(t)for(const i of e.hand.values())this._getHandJoint(t,i)}return this.dispatchEvent({type:"connected",data:e}),this}disconnect(e){return this.dispatchEvent({type:"disconnected",data:e}),this._targetRay!==null&&(this._targetRay.visible=!1),this._grip!==null&&(this._grip.visible=!1),this._hand!==null&&(this._hand.visible=!1),this}update(e,t,i){let o=null,a=null,u=null;const c=this._targetRay,h=this._grip,d=this._hand;if(e&&t.session.visibilityState!=="visible-blurred"){if(d&&e.hand){u=!0;for(const E of e.hand.values()){const y=t.getJointPose(E,i),x=this._getHandJoint(d,E);y!==null&&(x.matrix.fromArray(y.transform.matrix),x.matrix.decompose(x.position,x.rotation,x.scale),x.matrixWorldNeedsUpdate=!0,x.jointRadius=y.radius),x.visible=y!==null}const m=d.joints["index-finger-tip"],_=d.joints["thumb-tip"],g=m.position.distanceTo(_.position),S=.02,M=.005;d.inputState.pinching&&g>S+M?(d.inputState.pinching=!1,this.dispatchEvent({type:"pinchend",handedness:e.handedness,target:this})):!d.inputState.pinching&&g<=S-M&&(d.inputState.pinching=!0,this.dispatchEvent({type:"pinchstart",handedness:e.handedness,target:this}))}else h!==null&&e.gripSpace&&(a=t.getPose(e.gripSpace,i),a!==null&&(h.matrix.fromArray(a.transform.matrix),h.matrix.decompose(h.position,h.rotation,h.scale),h.matrixWorldNeedsUpdate=!0,a.linearVelocity?(h.hasLinearVelocity=!0,h.linearVelocity.copy(a.linearVelocity)):h.hasLinearVelocity=!1,a.angularVelocity?(h.hasAngularVelocity=!0,h.angularVelocity.copy(a.angularVelocity)):h.hasAngularVelocity=!1));c!==null&&(o=t.getPose(e.targetRaySpace,i),o===null&&a!==null&&(o=a),o!==null&&(c.matrix.fromArray(o.transform.matrix),c.matrix.decompose(c.position,c.rotation,c.scale),c.matrixWorldNeedsUpdate=!0,o.linearVelocity?(c.hasLinearVelocity=!0,c.linearVelocity.copy(o.linearVelocity)):c.hasLinearVelocity=!1,o.angularVelocity?(c.hasAngularVelocity=!0,c.angularVelocity.copy(o.angularVelocity)):c.hasAngularVelocity=!1,this.dispatchEvent(ST)))}return c!==null&&(c.visible=o!==null),h!==null&&(h.visible=a!==null),d!==null&&(d.visible=u!==null),this}_getHandJoint(e,t){if(e.joints[t.jointName]===void 0){const i=new qi;i.matrixAutoUpdate=!1,i.visible=!1,e.joints[t.jointName]=i,e.add(i)}return e.joints[t.jointName]}}const MT=`
void main() {

	gl_Position = vec4( position, 1.0 );

}`,ET=`
uniform sampler2DArray depthColor;
uniform float depthWidth;
uniform float depthHeight;

void main() {

	vec2 coord = vec2( gl_FragCoord.x / depthWidth, gl_FragCoord.y / depthHeight );

	if ( coord.x >= 1.0 ) {

		gl_FragDepth = texture( depthColor, vec3( coord.x - 1.0, coord.y, 1 ) ).r;

	} else {

		gl_FragDepth = texture( depthColor, vec3( coord.x, coord.y, 0 ) ).r;

	}

}`;class wT{constructor(){this.texture=null,this.mesh=null,this.depthNear=0,this.depthFar=0}init(e,t,i){if(this.texture===null){const o=new Rn,a=e.properties.get(o);a.__webglTexture=t.texture,(t.depthNear!=i.depthNear||t.depthFar!=i.depthFar)&&(this.depthNear=t.depthNear,this.depthFar=t.depthFar),this.texture=o}}getMesh(e){if(this.texture!==null&&this.mesh===null){const t=e.cameras[0].viewport,i=new Nr({vertexShader:MT,fragmentShader:ET,uniforms:{depthColor:{value:this.texture},depthWidth:{value:t.z},depthHeight:{value:t.w}}});this.mesh=new zt(new Ra(20,20),i)}return this.mesh}reset(){this.texture=null,this.mesh=null}getDepthTexture(){return this.texture}}class TT extends hs{constructor(e,t){super();const i=this;let o=null,a=1,u=null,c="local-floor",h=1,d=null,m=null,_=null,g=null,S=null,M=null;const E=new wT,y=t.getContextAttributes();let x=null,b=null;const T=[],P=[],H=new Pe;let D=null;const N=new ni;N.layers.enable(1),N.viewport=new Qt;const z=new ni;z.layers.enable(2),z.viewport=new Qt;const C=[N,z],w=new xT;w.layers.enable(1),w.layers.enable(2);let U=null,X=null;this.cameraAutoUpdate=!0,this.enabled=!1,this.isPresenting=!1,this.getController=function(Z){let ne=T[Z];return ne===void 0&&(ne=new _h,T[Z]=ne),ne.getTargetRaySpace()},this.getControllerGrip=function(Z){let ne=T[Z];return ne===void 0&&(ne=new _h,T[Z]=ne),ne.getGripSpace()},this.getHand=function(Z){let ne=T[Z];return ne===void 0&&(ne=new _h,T[Z]=ne),ne.getHandSpace()};function k(Z){const ne=P.indexOf(Z.inputSource);if(ne===-1)return;const xe=T[ne];xe!==void 0&&(xe.update(Z.inputSource,Z.frame,d||u),xe.dispatchEvent({type:Z.type,data:Z.inputSource}))}function Y(){o.removeEventListener("select",k),o.removeEventListener("selectstart",k),o.removeEventListener("selectend",k),o.removeEventListener("squeeze",k),o.removeEventListener("squeezestart",k),o.removeEventListener("squeezeend",k),o.removeEventListener("end",Y),o.removeEventListener("inputsourceschange",ae);for(let Z=0;Z<T.length;Z++){const ne=P[Z];ne!==null&&(P[Z]=null,T[Z].disconnect(ne))}U=null,X=null,E.reset(),e.setRenderTarget(x),S=null,g=null,_=null,o=null,b=null,Ae.stop(),i.isPresenting=!1,e.setPixelRatio(D),e.setSize(H.width,H.height,!1),i.dispatchEvent({type:"sessionend"})}this.setFramebufferScaleFactor=function(Z){a=Z,i.isPresenting===!0&&console.warn("THREE.WebXRManager: Cannot change framebuffer scale while presenting.")},this.setReferenceSpaceType=function(Z){c=Z,i.isPresenting===!0&&console.warn("THREE.WebXRManager: Cannot change reference space type while presenting.")},this.getReferenceSpace=function(){return d||u},this.setReferenceSpace=function(Z){d=Z},this.getBaseLayer=function(){return g!==null?g:S},this.getBinding=function(){return _},this.getFrame=function(){return M},this.getSession=function(){return o},this.setSession=async function(Z){if(o=Z,o!==null){if(x=e.getRenderTarget(),o.addEventListener("select",k),o.addEventListener("selectstart",k),o.addEventListener("selectend",k),o.addEventListener("squeeze",k),o.addEventListener("squeezestart",k),o.addEventListener("squeezeend",k),o.addEventListener("end",Y),o.addEventListener("inputsourceschange",ae),y.xrCompatible!==!0&&await t.makeXRCompatible(),D=e.getPixelRatio(),e.getSize(H),o.renderState.layers===void 0){const ne={antialias:y.antialias,alpha:!0,depth:y.depth,stencil:y.stencil,framebufferScaleFactor:a};S=new XRWebGLLayer(o,t,ne),o.updateRenderState({baseLayer:S}),e.setPixelRatio(1),e.setSize(S.framebufferWidth,S.framebufferHeight,!1),b=new as(S.framebufferWidth,S.framebufferHeight,{format:mi,type:Ki,colorSpace:e.outputColorSpace,stencilBuffer:y.stencil})}else{let ne=null,xe=null,de=null;y.depth&&(de=y.stencil?t.DEPTH24_STENCIL8:t.DEPTH_COMPONENT24,ne=y.stencil?Mo:mo,xe=y.stencil?So:os);const ee={colorFormat:t.RGBA8,depthFormat:de,scaleFactor:a};_=new XRWebGLBinding(o,t),g=_.createProjectionLayer(ee),o.updateRenderState({layers:[g]}),e.setPixelRatio(1),e.setSize(g.textureWidth,g.textureHeight,!1),b=new as(g.textureWidth,g.textureHeight,{format:mi,type:Ki,depthTexture:new F_(g.textureWidth,g.textureHeight,xe,void 0,void 0,void 0,void 0,void 0,void 0,ne),stencilBuffer:y.stencil,colorSpace:e.outputColorSpace,samples:y.antialias?4:0,resolveDepthBuffer:g.ignoreDepthValues===!1})}b.isXRRenderTarget=!0,this.setFoveation(h),d=null,u=await o.requestReferenceSpace(c),Ae.setContext(o),Ae.start(),i.isPresenting=!0,i.dispatchEvent({type:"sessionstart"})}},this.getEnvironmentBlendMode=function(){if(o!==null)return o.environmentBlendMode},this.getDepthTexture=function(){return E.getDepthTexture()};function ae(Z){for(let ne=0;ne<Z.removed.length;ne++){const xe=Z.removed[ne],de=P.indexOf(xe);de>=0&&(P[de]=null,T[de].disconnect(xe))}for(let ne=0;ne<Z.added.length;ne++){const xe=Z.added[ne];let de=P.indexOf(xe);if(de===-1){for(let we=0;we<T.length;we++)if(we>=P.length){P.push(xe),de=we;break}else if(P[we]===null){P[we]=xe,de=we;break}if(de===-1)break}const ee=T[de];ee&&ee.connect(xe)}}const te=new W,ce=new W;function G(Z,ne,xe){te.setFromMatrixPosition(ne.matrixWorld),ce.setFromMatrixPosition(xe.matrixWorld);const de=te.distanceTo(ce),ee=ne.projectionMatrix.elements,we=xe.projectionMatrix.elements,Ue=ee[14]/(ee[10]-1),tt=ee[14]/(ee[10]+1),I=(ee[9]+1)/ee[5],Te=(ee[9]-1)/ee[5],Se=(ee[8]-1)/ee[0],Oe=(we[8]+1)/we[0],pe=Ue*Se,et=Ue*Oe,Be=de/(-Se+Oe),We=Be*-Se;if(ne.matrixWorld.decompose(Z.position,Z.quaternion,Z.scale),Z.translateX(We),Z.translateZ(Be),Z.matrixWorld.compose(Z.position,Z.quaternion,Z.scale),Z.matrixWorldInverse.copy(Z.matrixWorld).invert(),ee[10]===-1)Z.projectionMatrix.copy(ne.projectionMatrix),Z.projectionMatrixInverse.copy(ne.projectionMatrixInverse);else{const F=Ue+Be,R=tt+Be,oe=pe-We,ve=et+(de-We),_e=I*tt/R*F,ye=Te*tt/R*F;Z.projectionMatrix.makePerspective(oe,ve,_e,ye,F,R),Z.projectionMatrixInverse.copy(Z.projectionMatrix).invert()}}function he(Z,ne){ne===null?Z.matrixWorld.copy(Z.matrix):Z.matrixWorld.multiplyMatrices(ne.matrixWorld,Z.matrix),Z.matrixWorldInverse.copy(Z.matrixWorld).invert()}this.updateCamera=function(Z){if(o===null)return;let ne=Z.near,xe=Z.far;E.texture!==null&&(E.depthNear>0&&(ne=E.depthNear),E.depthFar>0&&(xe=E.depthFar)),w.near=z.near=N.near=ne,w.far=z.far=N.far=xe,(U!==w.near||X!==w.far)&&(o.updateRenderState({depthNear:w.near,depthFar:w.far}),U=w.near,X=w.far);const de=Z.parent,ee=w.cameras;he(w,de);for(let we=0;we<ee.length;we++)he(ee[we],de);ee.length===2?G(w,N,z):w.projectionMatrix.copy(N.projectionMatrix),re(Z,w,de)};function re(Z,ne,xe){xe===null?Z.matrix.copy(ne.matrixWorld):(Z.matrix.copy(xe.matrixWorld),Z.matrix.invert(),Z.matrix.multiply(ne.matrixWorld)),Z.matrix.decompose(Z.position,Z.quaternion,Z.scale),Z.updateMatrixWorld(!0),Z.projectionMatrix.copy(ne.projectionMatrix),Z.projectionMatrixInverse.copy(ne.projectionMatrixInverse),Z.isPerspectiveCamera&&(Z.fov=sd*2*Math.atan(1/Z.projectionMatrix.elements[5]),Z.zoom=1)}this.getCamera=function(){return w},this.getFoveation=function(){if(!(g===null&&S===null))return h},this.setFoveation=function(Z){h=Z,g!==null&&(g.fixedFoveation=Z),S!==null&&S.fixedFoveation!==void 0&&(S.fixedFoveation=Z)},this.hasDepthSensing=function(){return E.texture!==null},this.getDepthSensingMesh=function(){return E.getMesh(w)};let O=null;function ie(Z,ne){if(m=ne.getViewerPose(d||u),M=ne,m!==null){const xe=m.views;S!==null&&(e.setRenderTargetFramebuffer(b,S.framebuffer),e.setRenderTarget(b));let de=!1;xe.length!==w.cameras.length&&(w.cameras.length=0,de=!0);for(let we=0;we<xe.length;we++){const Ue=xe[we];let tt=null;if(S!==null)tt=S.getViewport(Ue);else{const Te=_.getViewSubImage(g,Ue);tt=Te.viewport,we===0&&(e.setRenderTargetTextures(b,Te.colorTexture,g.ignoreDepthValues?void 0:Te.depthStencilTexture),e.setRenderTarget(b))}let I=C[we];I===void 0&&(I=new ni,I.layers.enable(we),I.viewport=new Qt,C[we]=I),I.matrix.fromArray(Ue.transform.matrix),I.matrix.decompose(I.position,I.quaternion,I.scale),I.projectionMatrix.fromArray(Ue.projectionMatrix),I.projectionMatrixInverse.copy(I.projectionMatrix).invert(),I.viewport.set(tt.x,tt.y,tt.width,tt.height),we===0&&(w.matrix.copy(I.matrix),w.matrix.decompose(w.position,w.quaternion,w.scale)),de===!0&&w.cameras.push(I)}const ee=o.enabledFeatures;if(ee&&ee.includes("depth-sensing")){const we=_.getDepthInformation(xe[0]);we&&we.isValid&&we.texture&&E.init(e,we,o.renderState)}}for(let xe=0;xe<T.length;xe++){const de=P[xe],ee=T[xe];de!==null&&ee!==void 0&&ee.update(de,ne,d||u)}O&&O(Z,ne),ne.detectedPlanes&&i.dispatchEvent({type:"planesdetected",data:ne}),M=null}const Ae=new I_;Ae.setAnimationLoop(ie),this.setAnimationLoop=function(Z){O=Z},this.dispose=function(){}}}const es=new Ai,AT=new kt;function CT(s,e){function t(y,x){y.matrixAutoUpdate===!0&&y.updateMatrix(),x.value.copy(y.matrix)}function i(y,x){x.color.getRGB(y.fogColor.value,L_(s)),x.isFog?(y.fogNear.value=x.near,y.fogFar.value=x.far):x.isFogExp2&&(y.fogDensity.value=x.density)}function o(y,x,b,T,P){x.isMeshBasicMaterial||x.isMeshLambertMaterial?a(y,x):x.isMeshToonMaterial?(a(y,x),_(y,x)):x.isMeshPhongMaterial?(a(y,x),m(y,x)):x.isMeshStandardMaterial?(a(y,x),g(y,x),x.isMeshPhysicalMaterial&&S(y,x,P)):x.isMeshMatcapMaterial?(a(y,x),M(y,x)):x.isMeshDepthMaterial?a(y,x):x.isMeshDistanceMaterial?(a(y,x),E(y,x)):x.isMeshNormalMaterial?a(y,x):x.isLineBasicMaterial?(u(y,x),x.isLineDashedMaterial&&c(y,x)):x.isPointsMaterial?h(y,x,b,T):x.isSpriteMaterial?d(y,x):x.isShadowMaterial?(y.color.value.copy(x.color),y.opacity.value=x.opacity):x.isShaderMaterial&&(x.uniformsNeedUpdate=!1)}function a(y,x){y.opacity.value=x.opacity,x.color&&y.diffuse.value.copy(x.color),x.emissive&&y.emissive.value.copy(x.emissive).multiplyScalar(x.emissiveIntensity),x.map&&(y.map.value=x.map,t(x.map,y.mapTransform)),x.alphaMap&&(y.alphaMap.value=x.alphaMap,t(x.alphaMap,y.alphaMapTransform)),x.bumpMap&&(y.bumpMap.value=x.bumpMap,t(x.bumpMap,y.bumpMapTransform),y.bumpScale.value=x.bumpScale,x.side===Bn&&(y.bumpScale.value*=-1)),x.normalMap&&(y.normalMap.value=x.normalMap,t(x.normalMap,y.normalMapTransform),y.normalScale.value.copy(x.normalScale),x.side===Bn&&y.normalScale.value.negate()),x.displacementMap&&(y.displacementMap.value=x.displacementMap,t(x.displacementMap,y.displacementMapTransform),y.displacementScale.value=x.displacementScale,y.displacementBias.value=x.displacementBias),x.emissiveMap&&(y.emissiveMap.value=x.emissiveMap,t(x.emissiveMap,y.emissiveMapTransform)),x.specularMap&&(y.specularMap.value=x.specularMap,t(x.specularMap,y.specularMapTransform)),x.alphaTest>0&&(y.alphaTest.value=x.alphaTest);const b=e.get(x),T=b.envMap,P=b.envMapRotation;T&&(y.envMap.value=T,es.copy(P),es.x*=-1,es.y*=-1,es.z*=-1,T.isCubeTexture&&T.isRenderTargetTexture===!1&&(es.y*=-1,es.z*=-1),y.envMapRotation.value.setFromMatrix4(AT.makeRotationFromEuler(es)),y.flipEnvMap.value=T.isCubeTexture&&T.isRenderTargetTexture===!1?-1:1,y.reflectivity.value=x.reflectivity,y.ior.value=x.ior,y.refractionRatio.value=x.refractionRatio),x.lightMap&&(y.lightMap.value=x.lightMap,y.lightMapIntensity.value=x.lightMapIntensity,t(x.lightMap,y.lightMapTransform)),x.aoMap&&(y.aoMap.value=x.aoMap,y.aoMapIntensity.value=x.aoMapIntensity,t(x.aoMap,y.aoMapTransform))}function u(y,x){y.diffuse.value.copy(x.color),y.opacity.value=x.opacity,x.map&&(y.map.value=x.map,t(x.map,y.mapTransform))}function c(y,x){y.dashSize.value=x.dashSize,y.totalSize.value=x.dashSize+x.gapSize,y.scale.value=x.scale}function h(y,x,b,T){y.diffuse.value.copy(x.color),y.opacity.value=x.opacity,y.size.value=x.size*b,y.scale.value=T*.5,x.map&&(y.map.value=x.map,t(x.map,y.uvTransform)),x.alphaMap&&(y.alphaMap.value=x.alphaMap,t(x.alphaMap,y.alphaMapTransform)),x.alphaTest>0&&(y.alphaTest.value=x.alphaTest)}function d(y,x){y.diffuse.value.copy(x.color),y.opacity.value=x.opacity,y.rotation.value=x.rotation,x.map&&(y.map.value=x.map,t(x.map,y.mapTransform)),x.alphaMap&&(y.alphaMap.value=x.alphaMap,t(x.alphaMap,y.alphaMapTransform)),x.alphaTest>0&&(y.alphaTest.value=x.alphaTest)}function m(y,x){y.specular.value.copy(x.specular),y.shininess.value=Math.max(x.shininess,1e-4)}function _(y,x){x.gradientMap&&(y.gradientMap.value=x.gradientMap)}function g(y,x){y.metalness.value=x.metalness,x.metalnessMap&&(y.metalnessMap.value=x.metalnessMap,t(x.metalnessMap,y.metalnessMapTransform)),y.roughness.value=x.roughness,x.roughnessMap&&(y.roughnessMap.value=x.roughnessMap,t(x.roughnessMap,y.roughnessMapTransform)),x.envMap&&(y.envMapIntensity.value=x.envMapIntensity)}function S(y,x,b){y.ior.value=x.ior,x.sheen>0&&(y.sheenColor.value.copy(x.sheenColor).multiplyScalar(x.sheen),y.sheenRoughness.value=x.sheenRoughness,x.sheenColorMap&&(y.sheenColorMap.value=x.sheenColorMap,t(x.sheenColorMap,y.sheenColorMapTransform)),x.sheenRoughnessMap&&(y.sheenRoughnessMap.value=x.sheenRoughnessMap,t(x.sheenRoughnessMap,y.sheenRoughnessMapTransform))),x.clearcoat>0&&(y.clearcoat.value=x.clearcoat,y.clearcoatRoughness.value=x.clearcoatRoughness,x.clearcoatMap&&(y.clearcoatMap.value=x.clearcoatMap,t(x.clearcoatMap,y.clearcoatMapTransform)),x.clearcoatRoughnessMap&&(y.clearcoatRoughnessMap.value=x.clearcoatRoughnessMap,t(x.clearcoatRoughnessMap,y.clearcoatRoughnessMapTransform)),x.clearcoatNormalMap&&(y.clearcoatNormalMap.value=x.clearcoatNormalMap,t(x.clearcoatNormalMap,y.clearcoatNormalMapTransform),y.clearcoatNormalScale.value.copy(x.clearcoatNormalScale),x.side===Bn&&y.clearcoatNormalScale.value.negate())),x.dispersion>0&&(y.dispersion.value=x.dispersion),x.iridescence>0&&(y.iridescence.value=x.iridescence,y.iridescenceIOR.value=x.iridescenceIOR,y.iridescenceThicknessMinimum.value=x.iridescenceThicknessRange[0],y.iridescenceThicknessMaximum.value=x.iridescenceThicknessRange[1],x.iridescenceMap&&(y.iridescenceMap.value=x.iridescenceMap,t(x.iridescenceMap,y.iridescenceMapTransform)),x.iridescenceThicknessMap&&(y.iridescenceThicknessMap.value=x.iridescenceThicknessMap,t(x.iridescenceThicknessMap,y.iridescenceThicknessMapTransform))),x.transmission>0&&(y.transmission.value=x.transmission,y.transmissionSamplerMap.value=b.texture,y.transmissionSamplerSize.value.set(b.width,b.height),x.transmissionMap&&(y.transmissionMap.value=x.transmissionMap,t(x.transmissionMap,y.transmissionMapTransform)),y.thickness.value=x.thickness,x.thicknessMap&&(y.thicknessMap.value=x.thicknessMap,t(x.thicknessMap,y.thicknessMapTransform)),y.attenuationDistance.value=x.attenuationDistance,y.attenuationColor.value.copy(x.attenuationColor)),x.anisotropy>0&&(y.anisotropyVector.value.set(x.anisotropy*Math.cos(x.anisotropyRotation),x.anisotropy*Math.sin(x.anisotropyRotation)),x.anisotropyMap&&(y.anisotropyMap.value=x.anisotropyMap,t(x.anisotropyMap,y.anisotropyMapTransform))),y.specularIntensity.value=x.specularIntensity,y.specularColor.value.copy(x.specularColor),x.specularColorMap&&(y.specularColorMap.value=x.specularColorMap,t(x.specularColorMap,y.specularColorMapTransform)),x.specularIntensityMap&&(y.specularIntensityMap.value=x.specularIntensityMap,t(x.specularIntensityMap,y.specularIntensityMapTransform))}function M(y,x){x.matcap&&(y.matcap.value=x.matcap)}function E(y,x){const b=e.get(x).light;y.referencePosition.value.setFromMatrixPosition(b.matrixWorld),y.nearDistance.value=b.shadow.camera.near,y.farDistance.value=b.shadow.camera.far}return{refreshFogUniforms:i,refreshMaterialUniforms:o}}function RT(s,e,t,i){let o={},a={},u=[];const c=s.getParameter(s.MAX_UNIFORM_BUFFER_BINDINGS);function h(b,T){const P=T.program;i.uniformBlockBinding(b,P)}function d(b,T){let P=o[b.id];P===void 0&&(M(b),P=m(b),o[b.id]=P,b.addEventListener("dispose",y));const H=T.program;i.updateUBOMapping(b,H);const D=e.render.frame;a[b.id]!==D&&(g(b),a[b.id]=D)}function m(b){const T=_();b.__bindingPointIndex=T;const P=s.createBuffer(),H=b.__size,D=b.usage;return s.bindBuffer(s.UNIFORM_BUFFER,P),s.bufferData(s.UNIFORM_BUFFER,H,D),s.bindBuffer(s.UNIFORM_BUFFER,null),s.bindBufferBase(s.UNIFORM_BUFFER,T,P),P}function _(){for(let b=0;b<c;b++)if(u.indexOf(b)===-1)return u.push(b),b;return console.error("THREE.WebGLRenderer: Maximum number of simultaneously usable uniforms groups reached."),0}function g(b){const T=o[b.id],P=b.uniforms,H=b.__cache;s.bindBuffer(s.UNIFORM_BUFFER,T);for(let D=0,N=P.length;D<N;D++){const z=Array.isArray(P[D])?P[D]:[P[D]];for(let C=0,w=z.length;C<w;C++){const U=z[C];if(S(U,D,C,H)===!0){const X=U.__offset,k=Array.isArray(U.value)?U.value:[U.value];let Y=0;for(let ae=0;ae<k.length;ae++){const te=k[ae],ce=E(te);typeof te=="number"||typeof te=="boolean"?(U.__data[0]=te,s.bufferSubData(s.UNIFORM_BUFFER,X+Y,U.__data)):te.isMatrix3?(U.__data[0]=te.elements[0],U.__data[1]=te.elements[1],U.__data[2]=te.elements[2],U.__data[3]=0,U.__data[4]=te.elements[3],U.__data[5]=te.elements[4],U.__data[6]=te.elements[5],U.__data[7]=0,U.__data[8]=te.elements[6],U.__data[9]=te.elements[7],U.__data[10]=te.elements[8],U.__data[11]=0):(te.toArray(U.__data,Y),Y+=ce.storage/Float32Array.BYTES_PER_ELEMENT)}s.bufferSubData(s.UNIFORM_BUFFER,X,U.__data)}}}s.bindBuffer(s.UNIFORM_BUFFER,null)}function S(b,T,P,H){const D=b.value,N=T+"_"+P;if(H[N]===void 0)return typeof D=="number"||typeof D=="boolean"?H[N]=D:H[N]=D.clone(),!0;{const z=H[N];if(typeof D=="number"||typeof D=="boolean"){if(z!==D)return H[N]=D,!0}else if(z.equals(D)===!1)return z.copy(D),!0}return!1}function M(b){const T=b.uniforms;let P=0;const H=16;for(let N=0,z=T.length;N<z;N++){const C=Array.isArray(T[N])?T[N]:[T[N]];for(let w=0,U=C.length;w<U;w++){const X=C[w],k=Array.isArray(X.value)?X.value:[X.value];for(let Y=0,ae=k.length;Y<ae;Y++){const te=k[Y],ce=E(te),G=P%H,he=G%ce.boundary,re=G+he;P+=he,re!==0&&H-re<ce.storage&&(P+=H-re),X.__data=new Float32Array(ce.storage/Float32Array.BYTES_PER_ELEMENT),X.__offset=P,P+=ce.storage}}}const D=P%H;return D>0&&(P+=H-D),b.__size=P,b.__cache={},this}function E(b){const T={boundary:0,storage:0};return typeof b=="number"||typeof b=="boolean"?(T.boundary=4,T.storage=4):b.isVector2?(T.boundary=8,T.storage=8):b.isVector3||b.isColor?(T.boundary=16,T.storage=12):b.isVector4?(T.boundary=16,T.storage=16):b.isMatrix3?(T.boundary=48,T.storage=48):b.isMatrix4?(T.boundary=64,T.storage=64):b.isTexture?console.warn("THREE.WebGLRenderer: Texture samplers can not be part of an uniforms group."):console.warn("THREE.WebGLRenderer: Unsupported uniform value type.",b),T}function y(b){const T=b.target;T.removeEventListener("dispose",y);const P=u.indexOf(T.__bindingPointIndex);u.splice(P,1),s.deleteBuffer(o[T.id]),delete o[T.id],delete a[T.id]}function x(){for(const b in o)s.deleteBuffer(o[b]);u=[],o={},a={}}return{bind:h,update:d,dispose:x}}class PT{constructor(e={}){const{canvas:t=yS(),context:i=null,depth:o=!0,stencil:a=!1,alpha:u=!1,antialias:c=!1,premultipliedAlpha:h=!0,preserveDrawingBuffer:d=!1,powerPreference:m="default",failIfMajorPerformanceCaveat:_=!1}=e;this.isWebGLRenderer=!0;let g;if(i!==null){if(typeof WebGLRenderingContext<"u"&&i instanceof WebGLRenderingContext)throw new Error("THREE.WebGLRenderer: WebGL 1 is not supported since r163.");g=i.getContextAttributes().alpha}else g=u;const S=new Uint32Array(4),M=new Int32Array(4);let E=null,y=null;const x=[],b=[];this.domElement=t,this.debug={checkShaderErrors:!0,onShaderError:null},this.autoClear=!0,this.autoClearColor=!0,this.autoClearDepth=!0,this.autoClearStencil=!0,this.sortObjects=!0,this.clippingPlanes=[],this.localClippingEnabled=!1,this._outputColorSpace=Mi,this.toneMapping=Lr,this.toneMappingExposure=1;const T=this;let P=!1,H=0,D=0,N=null,z=-1,C=null;const w=new Qt,U=new Qt;let X=null;const k=new yt(0);let Y=0,ae=t.width,te=t.height,ce=1,G=null,he=null;const re=new Qt(0,0,ae,te),O=new Qt(0,0,ae,te);let ie=!1;const Ae=new Md;let Z=!1,ne=!1;const xe=new kt,de=new W,ee=new Qt,we={background:null,fog:null,environment:null,overrideMaterial:null,isScene:!0};let Ue=!1;function tt(){return N===null?ce:1}let I=i;function Te(L,K){return t.getContext(L,K)}try{const L={alpha:!0,depth:o,stencil:a,antialias:c,premultipliedAlpha:h,preserveDrawingBuffer:d,powerPreference:m,failIfMajorPerformanceCaveat:_};if("setAttribute"in t&&t.setAttribute("data-engine",`three.js r${dd}`),t.addEventListener("webglcontextlost",me,!1),t.addEventListener("webglcontextrestored",ge,!1),t.addEventListener("webglcontextcreationerror",Ne,!1),I===null){const K="webgl2";if(I=Te(K,L),I===null)throw Te(K)?new Error("Error creating WebGL context with your selected attributes."):new Error("Error creating WebGL context.")}}catch(L){throw console.error("THREE.WebGLRenderer: "+L.message),L}let Se,Oe,pe,et,Be,We,F,R,oe,ve,_e,ye,Qe,Fe,He,ut,Re,ke,_t,at,Xe,ct,ft,bt;function q(){Se=new U1(I),Se.init(),ct=new yT(I,Se),Oe=new P1(I,Se,e,ct),pe=new gT(I),et=new B1(I),Be=new nT,We=new vT(I,Se,pe,Be,Oe,ct,et),F=new L1(T),R=new I1(T),oe=new XS(I),ft=new C1(I,oe),ve=new F1(I,oe,et,ft),_e=new k1(I,ve,oe,et),_t=new z1(I,Oe,We),ut=new b1(Be),ye=new tT(T,F,R,Se,Oe,ft,ut),Qe=new CT(T,Be),Fe=new rT,He=new cT(Se),ke=new A1(T,F,R,pe,_e,g,h),Re=new mT(T,_e,Oe),bt=new RT(I,et,Oe,pe),at=new R1(I,Se,et),Xe=new O1(I,Se,et),et.programs=ye.programs,T.capabilities=Oe,T.extensions=Se,T.properties=Be,T.renderLists=Fe,T.shadowMap=Re,T.state=pe,T.info=et}q();const Le=new TT(T,I);this.xr=Le,this.getContext=function(){return I},this.getContextAttributes=function(){return I.getContextAttributes()},this.forceContextLoss=function(){const L=Se.get("WEBGL_lose_context");L&&L.loseContext()},this.forceContextRestore=function(){const L=Se.get("WEBGL_lose_context");L&&L.restoreContext()},this.getPixelRatio=function(){return ce},this.setPixelRatio=function(L){L!==void 0&&(ce=L,this.setSize(ae,te,!1))},this.getSize=function(L){return L.set(ae,te)},this.setSize=function(L,K,se=!0){if(Le.isPresenting){console.warn("THREE.WebGLRenderer: Can't change size while VR device is presenting.");return}ae=L,te=K,t.width=Math.floor(L*ce),t.height=Math.floor(K*ce),se===!0&&(t.style.width=L+"px",t.style.height=K+"px"),this.setViewport(0,0,L,K)},this.getDrawingBufferSize=function(L){return L.set(ae*ce,te*ce).floor()},this.setDrawingBufferSize=function(L,K,se){ae=L,te=K,ce=se,t.width=Math.floor(L*se),t.height=Math.floor(K*se),this.setViewport(0,0,L,K)},this.getCurrentViewport=function(L){return L.copy(w)},this.getViewport=function(L){return L.copy(re)},this.setViewport=function(L,K,se,le){L.isVector4?re.set(L.x,L.y,L.z,L.w):re.set(L,K,se,le),pe.viewport(w.copy(re).multiplyScalar(ce).round())},this.getScissor=function(L){return L.copy(O)},this.setScissor=function(L,K,se,le){L.isVector4?O.set(L.x,L.y,L.z,L.w):O.set(L,K,se,le),pe.scissor(U.copy(O).multiplyScalar(ce).round())},this.getScissorTest=function(){return ie},this.setScissorTest=function(L){pe.setScissorTest(ie=L)},this.setOpaqueSort=function(L){G=L},this.setTransparentSort=function(L){he=L},this.getClearColor=function(L){return L.copy(ke.getClearColor())},this.setClearColor=function(){ke.setClearColor.apply(ke,arguments)},this.getClearAlpha=function(){return ke.getClearAlpha()},this.setClearAlpha=function(){ke.setClearAlpha.apply(ke,arguments)},this.clear=function(L=!0,K=!0,se=!0){let le=0;if(L){let $=!1;if(N!==null){const De=N.texture.format;$=De===yd||De===vd||De===_d}if($){const De=N.texture.type,Ve=De===Ki||De===os||De===Sa||De===So||De===md||De===gd,Ye=ke.getClearColor(),Ie=ke.getClearAlpha(),ot=Ye.r,st=Ye.g,Ze=Ye.b;Ve?(S[0]=ot,S[1]=st,S[2]=Ze,S[3]=Ie,I.clearBufferuiv(I.COLOR,0,S)):(M[0]=ot,M[1]=st,M[2]=Ze,M[3]=Ie,I.clearBufferiv(I.COLOR,0,M))}else le|=I.COLOR_BUFFER_BIT}K&&(le|=I.DEPTH_BUFFER_BIT),se&&(le|=I.STENCIL_BUFFER_BIT,this.state.buffers.stencil.setMask(4294967295)),I.clear(le)},this.clearColor=function(){this.clear(!0,!1,!1)},this.clearDepth=function(){this.clear(!1,!0,!1)},this.clearStencil=function(){this.clear(!1,!1,!0)},this.dispose=function(){t.removeEventListener("webglcontextlost",me,!1),t.removeEventListener("webglcontextrestored",ge,!1),t.removeEventListener("webglcontextcreationerror",Ne,!1),Fe.dispose(),He.dispose(),Be.dispose(),F.dispose(),R.dispose(),_e.dispose(),ft.dispose(),bt.dispose(),ye.dispose(),Le.dispose(),Le.removeEventListener("sessionstart",kn),Le.removeEventListener("sessionend",$i),si.stop()};function me(L){L.preventDefault(),console.log("THREE.WebGLRenderer: Context Lost."),P=!0}function ge(){console.log("THREE.WebGLRenderer: Context Restored."),P=!1;const L=et.autoReset,K=Re.enabled,se=Re.autoUpdate,le=Re.needsUpdate,$=Re.type;q(),et.autoReset=L,Re.enabled=K,Re.autoUpdate=se,Re.needsUpdate=le,Re.type=$}function Ne(L){console.error("THREE.WebGLRenderer: A WebGL context could not be created. Reason: ",L.statusMessage)}function rt(L){const K=L.target;K.removeEventListener("dispose",rt),vt(K)}function vt(L){Ht(L),Be.remove(L)}function Ht(L){const K=Be.get(L).programs;K!==void 0&&(K.forEach(function(se){ye.releaseProgram(se)}),L.isShaderMaterial&&ye.releaseShaderCache(L))}this.renderBufferDirect=function(L,K,se,le,$,De){K===null&&(K=we);const Ve=$.isMesh&&$.matrixWorld.determinant()<0,Ye=Ou(L,K,se,le,$);pe.setMaterial(le,Ve);let Ie=se.index,ot=1;if(le.wireframe===!0){if(Ie=ve.getWireframeAttribute(se),Ie===void 0)return;ot=2}const st=se.drawRange,Ze=se.attributes.position;let xt=st.start*ot,Ot=(st.start+st.count)*ot;De!==null&&(xt=Math.max(xt,De.start*ot),Ot=Math.min(Ot,(De.start+De.count)*ot)),Ie!==null?(xt=Math.max(xt,0),Ot=Math.min(Ot,Ie.count)):Ze!=null&&(xt=Math.max(xt,0),Ot=Math.min(Ot,Ze.count));const Nt=Ot-xt;if(Nt<0||Nt===1/0)return;ft.setup($,le,Ye,se,Ie);let nn,ht=at;if(Ie!==null&&(nn=oe.get(Ie),ht=Xe,ht.setIndex(nn)),$.isMesh)le.wireframe===!0?(pe.setLineWidth(le.wireframeLinewidth*tt()),ht.setMode(I.LINES)):ht.setMode(I.TRIANGLES);else if($.isLine){let Je=le.linewidth;Je===void 0&&(Je=1),pe.setLineWidth(Je*tt()),$.isLineSegments?ht.setMode(I.LINES):$.isLineLoop?ht.setMode(I.LINE_LOOP):ht.setMode(I.LINE_STRIP)}else $.isPoints?ht.setMode(I.POINTS):$.isSprite&&ht.setMode(I.TRIANGLES);if($.isBatchedMesh)if($._multiDrawInstances!==null)ht.renderMultiDrawInstances($._multiDrawStarts,$._multiDrawCounts,$._multiDrawCount,$._multiDrawInstances);else if(Se.get("WEBGL_multi_draw"))ht.renderMultiDraw($._multiDrawStarts,$._multiDrawCounts,$._multiDrawCount);else{const Je=$._multiDrawStarts,At=$._multiDrawCounts,Et=$._multiDrawCount,Hn=Ie?oe.get(Ie).bytesPerElement:1,Qi=Be.get(le).currentProgram.getUniforms();for(let rn=0;rn<Et;rn++)Qi.setValue(I,"_gl_DrawID",rn),ht.render(Je[rn]/Hn,At[rn])}else if($.isInstancedMesh)ht.renderInstances(xt,Nt,$.count);else if(se.isInstancedBufferGeometry){const Je=se._maxInstanceCount!==void 0?se._maxInstanceCount:1/0,At=Math.min(se.instanceCount,Je);ht.renderInstances(xt,Nt,At)}else ht.render(xt,Nt)};function qt(L,K,se){L.transparent===!0&&L.side===wi&&L.forceSinglePass===!1?(L.side=Bn,L.needsUpdate=!0,Ji(L,K,se),L.side=Dr,L.needsUpdate=!0,Ji(L,K,se),L.side=wi):Ji(L,K,se)}this.compile=function(L,K,se=null){se===null&&(se=L),y=He.get(se),y.init(K),b.push(y),se.traverseVisible(function($){$.isLight&&$.layers.test(K.layers)&&(y.pushLight($),$.castShadow&&y.pushShadow($))}),L!==se&&L.traverseVisible(function($){$.isLight&&$.layers.test(K.layers)&&(y.pushLight($),$.castShadow&&y.pushShadow($))}),y.setupLights();const le=new Set;return L.traverse(function($){const De=$.material;if(De)if(Array.isArray(De))for(let Ve=0;Ve<De.length;Ve++){const Ye=De[Ve];qt(Ye,se,$),le.add(Ye)}else qt(De,se,$),le.add(De)}),b.pop(),y=null,le},this.compileAsync=function(L,K,se=null){const le=this.compile(L,K,se);return new Promise($=>{function De(){if(le.forEach(function(Ve){Be.get(Ve).currentProgram.isReady()&&le.delete(Ve)}),le.size===0){$(L);return}setTimeout(De,10)}Se.get("KHR_parallel_shader_compile")!==null?De():setTimeout(De,10)})};let St=null;function zn(L){St&&St(L)}function kn(){si.stop()}function $i(){si.start()}const si=new I_;si.setAnimationLoop(zn),typeof self<"u"&&si.setContext(self),this.setAnimationLoop=function(L){St=L,Le.setAnimationLoop(L),L===null?si.stop():si.start()},Le.addEventListener("sessionstart",kn),Le.addEventListener("sessionend",$i),this.render=function(L,K){if(K!==void 0&&K.isCamera!==!0){console.error("THREE.WebGLRenderer.render: camera is not an instance of THREE.Camera.");return}if(P===!0)return;if(L.matrixWorldAutoUpdate===!0&&L.updateMatrixWorld(),K.parent===null&&K.matrixWorldAutoUpdate===!0&&K.updateMatrixWorld(),Le.enabled===!0&&Le.isPresenting===!0&&(Le.cameraAutoUpdate===!0&&Le.updateCamera(K),K=Le.getCamera()),L.isScene===!0&&L.onBeforeRender(T,L,K,N),y=He.get(L,b.length),y.init(K),b.push(y),xe.multiplyMatrices(K.projectionMatrix,K.matrixWorldInverse),Ae.setFromProjectionMatrix(xe),ne=this.localClippingEnabled,Z=ut.init(this.clippingPlanes,ne),E=Fe.get(L,x.length),E.init(),x.push(E),Le.enabled===!0&&Le.isPresenting===!0){const De=T.xr.getDepthSensingMesh();De!==null&&Ri(De,K,-1/0,T.sortObjects)}Ri(L,K,0,T.sortObjects),E.finish(),T.sortObjects===!0&&E.sort(G,he),Ue=Le.enabled===!1||Le.isPresenting===!1||Le.hasDepthSensing()===!1,Ue&&ke.addToRenderList(E,L),this.info.render.frame++,Z===!0&&ut.beginShadows();const se=y.state.shadowsArray;Re.render(se,L,K),Z===!0&&ut.endShadows(),this.info.autoReset===!0&&this.info.reset();const le=E.opaque,$=E.transmissive;if(y.setupLights(),K.isArrayCamera){const De=K.cameras;if($.length>0)for(let Ve=0,Ye=De.length;Ve<Ye;Ve++){const Ie=De[Ve];Zi(le,$,L,Ie)}Ue&&ke.render(L);for(let Ve=0,Ye=De.length;Ve<Ye;Ve++){const Ie=De[Ve];Pa(E,L,Ie,Ie.viewport)}}else $.length>0&&Zi(le,$,L,K),Ue&&ke.render(L),Pa(E,L,K);N!==null&&(We.updateMultisampleRenderTarget(N),We.updateRenderTargetMipmap(N)),L.isScene===!0&&L.onAfterRender(T,L,K),ft.resetDefaultState(),z=-1,C=null,b.pop(),b.length>0?(y=b[b.length-1],Z===!0&&ut.setGlobalState(T.clippingPlanes,y.state.camera)):y=null,x.pop(),x.length>0?E=x[x.length-1]:E=null};function Ri(L,K,se,le){if(L.visible===!1)return;if(L.layers.test(K.layers)){if(L.isGroup)se=L.renderOrder;else if(L.isLOD)L.autoUpdate===!0&&L.update(K);else if(L.isLight)y.pushLight(L),L.castShadow&&y.pushShadow(L);else if(L.isSprite){if(!L.frustumCulled||Ae.intersectsSprite(L)){le&&ee.setFromMatrixPosition(L.matrixWorld).applyMatrix4(xe);const Ve=_e.update(L),Ye=L.material;Ye.visible&&E.push(L,Ve,Ye,se,ee.z,null)}}else if((L.isMesh||L.isLine||L.isPoints)&&(!L.frustumCulled||Ae.intersectsObject(L))){const Ve=_e.update(L),Ye=L.material;if(le&&(L.boundingSphere!==void 0?(L.boundingSphere===null&&L.computeBoundingSphere(),ee.copy(L.boundingSphere.center)):(Ve.boundingSphere===null&&Ve.computeBoundingSphere(),ee.copy(Ve.boundingSphere.center)),ee.applyMatrix4(L.matrixWorld).applyMatrix4(xe)),Array.isArray(Ye)){const Ie=Ve.groups;for(let ot=0,st=Ie.length;ot<st;ot++){const Ze=Ie[ot],xt=Ye[Ze.materialIndex];xt&&xt.visible&&E.push(L,Ve,xt,se,ee.z,Ze)}}else Ye.visible&&E.push(L,Ve,Ye,se,ee.z,null)}}const De=L.children;for(let Ve=0,Ye=De.length;Ve<Ye;Ve++)Ri(De[Ve],K,se,le)}function Pa(L,K,se,le){const $=L.opaque,De=L.transmissive,Ve=L.transparent;y.setupLightsView(se),Z===!0&&ut.setGlobalState(T.clippingPlanes,se),le&&pe.viewport(w.copy(le)),$.length>0&&Pi($,K,se),De.length>0&&Pi(De,K,se),Ve.length>0&&Pi(Ve,K,se),pe.buffers.depth.setTest(!0),pe.buffers.depth.setMask(!0),pe.buffers.color.setMask(!0),pe.setPolygonOffset(!1)}function Zi(L,K,se,le){if((se.isScene===!0?se.overrideMaterial:null)!==null)return;y.state.transmissionRenderTarget[le.id]===void 0&&(y.state.transmissionRenderTarget[le.id]=new as(1,1,{generateMipmaps:!0,type:Se.has("EXT_color_buffer_half_float")||Se.has("EXT_color_buffer_float")?Aa:Ki,minFilter:ss,samples:4,stencilBuffer:a,resolveDepthBuffer:!1,resolveStencilBuffer:!1,colorSpace:Tt.workingColorSpace}));const De=y.state.transmissionRenderTarget[le.id],Ve=le.viewport||w;De.setSize(Ve.z,Ve.w);const Ye=T.getRenderTarget();T.setRenderTarget(De),T.getClearColor(k),Y=T.getClearAlpha(),Y<1&&T.setClearColor(16777215,.5),T.clear(),Ue&&ke.render(se);const Ie=T.toneMapping;T.toneMapping=Lr;const ot=le.viewport;if(le.viewport!==void 0&&(le.viewport=void 0),y.setupLightsView(le),Z===!0&&ut.setGlobalState(T.clippingPlanes,le),Pi(L,se,le),We.updateMultisampleRenderTarget(De),We.updateRenderTargetMipmap(De),Se.has("WEBGL_multisampled_render_to_texture")===!1){let st=!1;for(let Ze=0,xt=K.length;Ze<xt;Ze++){const Ot=K[Ze],Nt=Ot.object,nn=Ot.geometry,ht=Ot.material,Je=Ot.group;if(ht.side===wi&&Nt.layers.test(le.layers)){const At=ht.side;ht.side=Bn,ht.needsUpdate=!0,Ur(Nt,se,le,nn,ht,Je),ht.side=At,ht.needsUpdate=!0,st=!0}}st===!0&&(We.updateMultisampleRenderTarget(De),We.updateRenderTargetMipmap(De))}T.setRenderTarget(Ye),T.setClearColor(k,Y),ot!==void 0&&(le.viewport=ot),T.toneMapping=Ie}function Pi(L,K,se){const le=K.isScene===!0?K.overrideMaterial:null;for(let $=0,De=L.length;$<De;$++){const Ve=L[$],Ye=Ve.object,Ie=Ve.geometry,ot=le===null?Ve.material:le,st=Ve.group;Ye.layers.test(se.layers)&&Ur(Ye,K,se,Ie,ot,st)}}function Ur(L,K,se,le,$,De){L.onBeforeRender(T,K,se,le,$,De),L.modelViewMatrix.multiplyMatrices(se.matrixWorldInverse,L.matrixWorld),L.normalMatrix.getNormalMatrix(L.modelViewMatrix),$.onBeforeRender(T,K,se,le,L,De),$.transparent===!0&&$.side===wi&&$.forceSinglePass===!1?($.side=Bn,$.needsUpdate=!0,T.renderBufferDirect(se,K,le,$,L,De),$.side=Dr,$.needsUpdate=!0,T.renderBufferDirect(se,K,le,$,L,De),$.side=wi):T.renderBufferDirect(se,K,le,$,L,De),L.onAfterRender(T,K,se,le,$,De)}function Ji(L,K,se){K.isScene!==!0&&(K=we);const le=Be.get(L),$=y.state.lights,De=y.state.shadowsArray,Ve=$.state.version,Ye=ye.getParameters(L,$.state,De,K,se),Ie=ye.getProgramCacheKey(Ye);let ot=le.programs;le.environment=L.isMeshStandardMaterial?K.environment:null,le.fog=K.fog,le.envMap=(L.isMeshStandardMaterial?R:F).get(L.envMap||le.environment),le.envMapRotation=le.environment!==null&&L.envMap===null?K.environmentRotation:L.envMapRotation,ot===void 0&&(L.addEventListener("dispose",rt),ot=new Map,le.programs=ot);let st=ot.get(Ie);if(st!==void 0){if(le.currentProgram===st&&le.lightsStateVersion===Ve)return La(L,Ye),st}else Ye.uniforms=ye.getUniforms(L),L.onBeforeCompile(Ye,T),st=ye.acquireProgram(Ye,Ie),ot.set(Ie,st),le.uniforms=Ye.uniforms;const Ze=le.uniforms;return(!L.isShaderMaterial&&!L.isRawShaderMaterial||L.clipping===!0)&&(Ze.clippingPlanes=ut.uniform),La(L,Ye),le.needsLights=Da(L),le.lightsStateVersion=Ve,le.needsLights&&(Ze.ambientLightColor.value=$.state.ambient,Ze.lightProbe.value=$.state.probe,Ze.directionalLights.value=$.state.directional,Ze.directionalLightShadows.value=$.state.directionalShadow,Ze.spotLights.value=$.state.spot,Ze.spotLightShadows.value=$.state.spotShadow,Ze.rectAreaLights.value=$.state.rectArea,Ze.ltc_1.value=$.state.rectAreaLTC1,Ze.ltc_2.value=$.state.rectAreaLTC2,Ze.pointLights.value=$.state.point,Ze.pointLightShadows.value=$.state.pointShadow,Ze.hemisphereLights.value=$.state.hemi,Ze.directionalShadowMap.value=$.state.directionalShadowMap,Ze.directionalShadowMatrix.value=$.state.directionalShadowMatrix,Ze.spotShadowMap.value=$.state.spotShadowMap,Ze.spotLightMatrix.value=$.state.spotLightMatrix,Ze.spotLightMap.value=$.state.spotLightMap,Ze.pointShadowMap.value=$.state.pointShadowMap,Ze.pointShadowMatrix.value=$.state.pointShadowMatrix),le.currentProgram=st,le.uniformsList=null,st}function ba(L){if(L.uniformsList===null){const K=L.currentProgram.getUniforms();L.uniformsList=Mu.seqWithValue(K.seq,L.uniforms)}return L.uniformsList}function La(L,K){const se=Be.get(L);se.outputColorSpace=K.outputColorSpace,se.batching=K.batching,se.batchingColor=K.batchingColor,se.instancing=K.instancing,se.instancingColor=K.instancingColor,se.instancingMorph=K.instancingMorph,se.skinning=K.skinning,se.morphTargets=K.morphTargets,se.morphNormals=K.morphNormals,se.morphColors=K.morphColors,se.morphTargetsCount=K.morphTargetsCount,se.numClippingPlanes=K.numClippingPlanes,se.numIntersection=K.numClipIntersection,se.vertexAlphas=K.vertexAlphas,se.vertexTangents=K.vertexTangents,se.toneMapping=K.toneMapping}function Ou(L,K,se,le,$){K.isScene!==!0&&(K=we),We.resetTextureUnits();const De=K.fog,Ve=le.isMeshStandardMaterial?K.environment:null,Ye=N===null?T.outputColorSpace:N.isXRRenderTarget===!0?N.texture.colorSpace:Ir,Ie=(le.isMeshStandardMaterial?R:F).get(le.envMap||Ve),ot=le.vertexColors===!0&&!!se.attributes.color&&se.attributes.color.itemSize===4,st=!!se.attributes.tangent&&(!!le.normalMap||le.anisotropy>0),Ze=!!se.morphAttributes.position,xt=!!se.morphAttributes.normal,Ot=!!se.morphAttributes.color;let Nt=Lr;le.toneMapped&&(N===null||N.isXRRenderTarget===!0)&&(Nt=T.toneMapping);const nn=se.morphAttributes.position||se.morphAttributes.normal||se.morphAttributes.color,ht=nn!==void 0?nn.length:0,Je=Be.get(le),At=y.state.lights;if(Z===!0&&(ne===!0||L!==C)){const Pn=L===C&&le.id===z;ut.setState(le,L,Pn)}let Et=!1;le.version===Je.__version?(Je.needsLights&&Je.lightsStateVersion!==At.state.version||Je.outputColorSpace!==Ye||$.isBatchedMesh&&Je.batching===!1||!$.isBatchedMesh&&Je.batching===!0||$.isBatchedMesh&&Je.batchingColor===!0&&$.colorTexture===null||$.isBatchedMesh&&Je.batchingColor===!1&&$.colorTexture!==null||$.isInstancedMesh&&Je.instancing===!1||!$.isInstancedMesh&&Je.instancing===!0||$.isSkinnedMesh&&Je.skinning===!1||!$.isSkinnedMesh&&Je.skinning===!0||$.isInstancedMesh&&Je.instancingColor===!0&&$.instanceColor===null||$.isInstancedMesh&&Je.instancingColor===!1&&$.instanceColor!==null||$.isInstancedMesh&&Je.instancingMorph===!0&&$.morphTexture===null||$.isInstancedMesh&&Je.instancingMorph===!1&&$.morphTexture!==null||Je.envMap!==Ie||le.fog===!0&&Je.fog!==De||Je.numClippingPlanes!==void 0&&(Je.numClippingPlanes!==ut.numPlanes||Je.numIntersection!==ut.numIntersection)||Je.vertexAlphas!==ot||Je.vertexTangents!==st||Je.morphTargets!==Ze||Je.morphNormals!==xt||Je.morphColors!==Ot||Je.toneMapping!==Nt||Je.morphTargetsCount!==ht)&&(Et=!0):(Et=!0,Je.__version=le.version);let Hn=Je.currentProgram;Et===!0&&(Hn=Ji(le,K,$));let Qi=!1,rn=!1,er=!1;const Lt=Hn.getUniforms(),Vn=Je.uniforms;if(pe.useProgram(Hn.program)&&(Qi=!0,rn=!0,er=!0),le.id!==z&&(z=le.id,rn=!0),Qi||C!==L){Lt.setValue(I,"projectionMatrix",L.projectionMatrix),Lt.setValue(I,"viewMatrix",L.matrixWorldInverse);const Pn=Lt.map.cameraPosition;Pn!==void 0&&Pn.setValue(I,de.setFromMatrixPosition(L.matrixWorld)),Oe.logarithmicDepthBuffer&&Lt.setValue(I,"logDepthBufFC",2/(Math.log(L.far+1)/Math.LN2)),(le.isMeshPhongMaterial||le.isMeshToonMaterial||le.isMeshLambertMaterial||le.isMeshBasicMaterial||le.isMeshStandardMaterial||le.isShaderMaterial)&&Lt.setValue(I,"isOrthographic",L.isOrthographicCamera===!0),C!==L&&(C=L,rn=!0,er=!0)}if($.isSkinnedMesh){Lt.setOptional(I,$,"bindMatrix"),Lt.setOptional(I,$,"bindMatrixInverse");const Pn=$.skeleton;Pn&&(Pn.boneTexture===null&&Pn.computeBoneTexture(),Lt.setValue(I,"boneTexture",Pn.boneTexture,We))}$.isBatchedMesh&&(Lt.setOptional(I,$,"batchingTexture"),Lt.setValue(I,"batchingTexture",$._matricesTexture,We),Lt.setOptional(I,$,"batchingIdTexture"),Lt.setValue(I,"batchingIdTexture",$._indirectTexture,We),Lt.setOptional(I,$,"batchingColorTexture"),$._colorsTexture!==null&&Lt.setValue(I,"batchingColorTexture",$._colorsTexture,We));const To=se.morphAttributes;if((To.position!==void 0||To.normal!==void 0||To.color!==void 0)&&_t.update($,se,Hn),(rn||Je.receiveShadow!==$.receiveShadow)&&(Je.receiveShadow=$.receiveShadow,Lt.setValue(I,"receiveShadow",$.receiveShadow)),le.isMeshGouraudMaterial&&le.envMap!==null&&(Vn.envMap.value=Ie,Vn.flipEnvMap.value=Ie.isCubeTexture&&Ie.isRenderTargetTexture===!1?-1:1),le.isMeshStandardMaterial&&le.envMap===null&&K.environment!==null&&(Vn.envMapIntensity.value=K.environmentIntensity),rn&&(Lt.setValue(I,"toneMappingExposure",T.toneMappingExposure),Je.needsLights&&bi(Vn,er),De&&le.fog===!0&&Qe.refreshFogUniforms(Vn,De),Qe.refreshMaterialUniforms(Vn,le,ce,te,y.state.transmissionRenderTarget[L.id]),Mu.upload(I,ba(Je),Vn,We)),le.isShaderMaterial&&le.uniformsNeedUpdate===!0&&(Mu.upload(I,ba(Je),Vn,We),le.uniformsNeedUpdate=!1),le.isSpriteMaterial&&Lt.setValue(I,"center",$.center),Lt.setValue(I,"modelViewMatrix",$.modelViewMatrix),Lt.setValue(I,"normalMatrix",$.normalMatrix),Lt.setValue(I,"modelMatrix",$.matrixWorld),le.isShaderMaterial||le.isRawShaderMaterial){const Pn=le.uniformsGroups;for(let Fr=0,Na=Pn.length;Fr<Na;Fr++){const ps=Pn[Fr];bt.update(ps,Hn),bt.bind(ps,Hn)}}return Hn}function bi(L,K){L.ambientLightColor.needsUpdate=K,L.lightProbe.needsUpdate=K,L.directionalLights.needsUpdate=K,L.directionalLightShadows.needsUpdate=K,L.pointLights.needsUpdate=K,L.pointLightShadows.needsUpdate=K,L.spotLights.needsUpdate=K,L.spotLightShadows.needsUpdate=K,L.rectAreaLights.needsUpdate=K,L.hemisphereLights.needsUpdate=K}function Da(L){return L.isMeshLambertMaterial||L.isMeshToonMaterial||L.isMeshPhongMaterial||L.isMeshStandardMaterial||L.isShadowMaterial||L.isShaderMaterial&&L.lights===!0}this.getActiveCubeFace=function(){return H},this.getActiveMipmapLevel=function(){return D},this.getRenderTarget=function(){return N},this.setRenderTargetTextures=function(L,K,se){Be.get(L.texture).__webglTexture=K,Be.get(L.depthTexture).__webglTexture=se;const le=Be.get(L);le.__hasExternalTextures=!0,le.__autoAllocateDepthBuffer=se===void 0,le.__autoAllocateDepthBuffer||Se.has("WEBGL_multisampled_render_to_texture")===!0&&(console.warn("THREE.WebGLRenderer: Render-to-texture extension was disabled because an external texture was provided"),le.__useRenderToTexture=!1)},this.setRenderTargetFramebuffer=function(L,K){const se=Be.get(L);se.__webglFramebuffer=K,se.__useDefaultFramebuffer=K===void 0},this.setRenderTarget=function(L,K=0,se=0){N=L,H=K,D=se;let le=!0,$=null,De=!1,Ve=!1;if(L){const Ie=Be.get(L);if(Ie.__useDefaultFramebuffer!==void 0)pe.bindFramebuffer(I.FRAMEBUFFER,null),le=!1;else if(Ie.__webglFramebuffer===void 0)We.setupRenderTarget(L);else if(Ie.__hasExternalTextures)We.rebindTextures(L,Be.get(L.texture).__webglTexture,Be.get(L.depthTexture).__webglTexture);else if(L.depthBuffer){const Ze=L.depthTexture;if(Ie.__boundDepthTexture!==Ze){if(Ze!==null&&Be.has(Ze)&&(L.width!==Ze.image.width||L.height!==Ze.image.height))throw new Error("WebGLRenderTarget: Attached DepthTexture is initialized to the incorrect size.");We.setupDepthRenderbuffer(L)}}const ot=L.texture;(ot.isData3DTexture||ot.isDataArrayTexture||ot.isCompressedArrayTexture)&&(Ve=!0);const st=Be.get(L).__webglFramebuffer;L.isWebGLCubeRenderTarget?(Array.isArray(st[K])?$=st[K][se]:$=st[K],De=!0):L.samples>0&&We.useMultisampledRTT(L)===!1?$=Be.get(L).__webglMultisampledFramebuffer:Array.isArray(st)?$=st[se]:$=st,w.copy(L.viewport),U.copy(L.scissor),X=L.scissorTest}else w.copy(re).multiplyScalar(ce).floor(),U.copy(O).multiplyScalar(ce).floor(),X=ie;if(pe.bindFramebuffer(I.FRAMEBUFFER,$)&&le&&pe.drawBuffers(L,$),pe.viewport(w),pe.scissor(U),pe.setScissorTest(X),De){const Ie=Be.get(L.texture);I.framebufferTexture2D(I.FRAMEBUFFER,I.COLOR_ATTACHMENT0,I.TEXTURE_CUBE_MAP_POSITIVE_X+K,Ie.__webglTexture,se)}else if(Ve){const Ie=Be.get(L.texture),ot=K||0;I.framebufferTextureLayer(I.FRAMEBUFFER,I.COLOR_ATTACHMENT0,Ie.__webglTexture,se||0,ot)}z=-1},this.readRenderTargetPixels=function(L,K,se,le,$,De,Ve){if(!(L&&L.isWebGLRenderTarget)){console.error("THREE.WebGLRenderer.readRenderTargetPixels: renderTarget is not THREE.WebGLRenderTarget.");return}let Ye=Be.get(L).__webglFramebuffer;if(L.isWebGLCubeRenderTarget&&Ve!==void 0&&(Ye=Ye[Ve]),Ye){pe.bindFramebuffer(I.FRAMEBUFFER,Ye);try{const Ie=L.texture,ot=Ie.format,st=Ie.type;if(!Oe.textureFormatReadable(ot)){console.error("THREE.WebGLRenderer.readRenderTargetPixels: renderTarget is not in RGBA or implementation defined format.");return}if(!Oe.textureTypeReadable(st)){console.error("THREE.WebGLRenderer.readRenderTargetPixels: renderTarget is not in UnsignedByteType or implementation defined type.");return}K>=0&&K<=L.width-le&&se>=0&&se<=L.height-$&&I.readPixels(K,se,le,$,ct.convert(ot),ct.convert(st),De)}finally{const Ie=N!==null?Be.get(N).__webglFramebuffer:null;pe.bindFramebuffer(I.FRAMEBUFFER,Ie)}}},this.readRenderTargetPixelsAsync=async function(L,K,se,le,$,De,Ve){if(!(L&&L.isWebGLRenderTarget))throw new Error("THREE.WebGLRenderer.readRenderTargetPixels: renderTarget is not THREE.WebGLRenderTarget.");let Ye=Be.get(L).__webglFramebuffer;if(L.isWebGLCubeRenderTarget&&Ve!==void 0&&(Ye=Ye[Ve]),Ye){pe.bindFramebuffer(I.FRAMEBUFFER,Ye);try{const Ie=L.texture,ot=Ie.format,st=Ie.type;if(!Oe.textureFormatReadable(ot))throw new Error("THREE.WebGLRenderer.readRenderTargetPixelsAsync: renderTarget is not in RGBA or implementation defined format.");if(!Oe.textureTypeReadable(st))throw new Error("THREE.WebGLRenderer.readRenderTargetPixelsAsync: renderTarget is not in UnsignedByteType or implementation defined type.");if(K>=0&&K<=L.width-le&&se>=0&&se<=L.height-$){const Ze=I.createBuffer();I.bindBuffer(I.PIXEL_PACK_BUFFER,Ze),I.bufferData(I.PIXEL_PACK_BUFFER,De.byteLength,I.STREAM_READ),I.readPixels(K,se,le,$,ct.convert(ot),ct.convert(st),0),I.flush();const xt=I.fenceSync(I.SYNC_GPU_COMMANDS_COMPLETE,0);await xS(I,xt,4);try{I.bindBuffer(I.PIXEL_PACK_BUFFER,Ze),I.getBufferSubData(I.PIXEL_PACK_BUFFER,0,De)}finally{I.deleteBuffer(Ze),I.deleteSync(xt)}return De}}finally{const Ie=N!==null?Be.get(N).__webglFramebuffer:null;pe.bindFramebuffer(I.FRAMEBUFFER,Ie)}}},this.copyFramebufferToTexture=function(L,K=null,se=0){L.isTexture!==!0&&(go("WebGLRenderer: copyFramebufferToTexture function signature has changed."),K=arguments[0]||null,L=arguments[1]);const le=Math.pow(2,-se),$=Math.floor(L.image.width*le),De=Math.floor(L.image.height*le),Ve=K!==null?K.x:0,Ye=K!==null?K.y:0;We.setTexture2D(L,0),I.copyTexSubImage2D(I.TEXTURE_2D,se,0,0,Ve,Ye,$,De),pe.unbindTexture()},this.copyTextureToTexture=function(L,K,se=null,le=null,$=0){L.isTexture!==!0&&(go("WebGLRenderer: copyTextureToTexture function signature has changed."),le=arguments[0]||null,L=arguments[1],K=arguments[2],$=arguments[3]||0,se=null);let De,Ve,Ye,Ie,ot,st;se!==null?(De=se.max.x-se.min.x,Ve=se.max.y-se.min.y,Ye=se.min.x,Ie=se.min.y):(De=L.image.width,Ve=L.image.height,Ye=0,Ie=0),le!==null?(ot=le.x,st=le.y):(ot=0,st=0);const Ze=ct.convert(K.format),xt=ct.convert(K.type);We.setTexture2D(K,0),I.pixelStorei(I.UNPACK_FLIP_Y_WEBGL,K.flipY),I.pixelStorei(I.UNPACK_PREMULTIPLY_ALPHA_WEBGL,K.premultiplyAlpha),I.pixelStorei(I.UNPACK_ALIGNMENT,K.unpackAlignment);const Ot=I.getParameter(I.UNPACK_ROW_LENGTH),Nt=I.getParameter(I.UNPACK_IMAGE_HEIGHT),nn=I.getParameter(I.UNPACK_SKIP_PIXELS),ht=I.getParameter(I.UNPACK_SKIP_ROWS),Je=I.getParameter(I.UNPACK_SKIP_IMAGES),At=L.isCompressedTexture?L.mipmaps[$]:L.image;I.pixelStorei(I.UNPACK_ROW_LENGTH,At.width),I.pixelStorei(I.UNPACK_IMAGE_HEIGHT,At.height),I.pixelStorei(I.UNPACK_SKIP_PIXELS,Ye),I.pixelStorei(I.UNPACK_SKIP_ROWS,Ie),L.isDataTexture?I.texSubImage2D(I.TEXTURE_2D,$,ot,st,De,Ve,Ze,xt,At.data):L.isCompressedTexture?I.compressedTexSubImage2D(I.TEXTURE_2D,$,ot,st,At.width,At.height,Ze,At.data):I.texSubImage2D(I.TEXTURE_2D,$,ot,st,De,Ve,Ze,xt,At),I.pixelStorei(I.UNPACK_ROW_LENGTH,Ot),I.pixelStorei(I.UNPACK_IMAGE_HEIGHT,Nt),I.pixelStorei(I.UNPACK_SKIP_PIXELS,nn),I.pixelStorei(I.UNPACK_SKIP_ROWS,ht),I.pixelStorei(I.UNPACK_SKIP_IMAGES,Je),$===0&&K.generateMipmaps&&I.generateMipmap(I.TEXTURE_2D),pe.unbindTexture()},this.copyTextureToTexture3D=function(L,K,se=null,le=null,$=0){L.isTexture!==!0&&(go("WebGLRenderer: copyTextureToTexture3D function signature has changed."),se=arguments[0]||null,le=arguments[1]||null,L=arguments[2],K=arguments[3],$=arguments[4]||0);let De,Ve,Ye,Ie,ot,st,Ze,xt,Ot;const Nt=L.isCompressedTexture?L.mipmaps[$]:L.image;se!==null?(De=se.max.x-se.min.x,Ve=se.max.y-se.min.y,Ye=se.max.z-se.min.z,Ie=se.min.x,ot=se.min.y,st=se.min.z):(De=Nt.width,Ve=Nt.height,Ye=Nt.depth,Ie=0,ot=0,st=0),le!==null?(Ze=le.x,xt=le.y,Ot=le.z):(Ze=0,xt=0,Ot=0);const nn=ct.convert(K.format),ht=ct.convert(K.type);let Je;if(K.isData3DTexture)We.setTexture3D(K,0),Je=I.TEXTURE_3D;else if(K.isDataArrayTexture||K.isCompressedArrayTexture)We.setTexture2DArray(K,0),Je=I.TEXTURE_2D_ARRAY;else{console.warn("THREE.WebGLRenderer.copyTextureToTexture3D: only supports THREE.DataTexture3D and THREE.DataTexture2DArray.");return}I.pixelStorei(I.UNPACK_FLIP_Y_WEBGL,K.flipY),I.pixelStorei(I.UNPACK_PREMULTIPLY_ALPHA_WEBGL,K.premultiplyAlpha),I.pixelStorei(I.UNPACK_ALIGNMENT,K.unpackAlignment);const At=I.getParameter(I.UNPACK_ROW_LENGTH),Et=I.getParameter(I.UNPACK_IMAGE_HEIGHT),Hn=I.getParameter(I.UNPACK_SKIP_PIXELS),Qi=I.getParameter(I.UNPACK_SKIP_ROWS),rn=I.getParameter(I.UNPACK_SKIP_IMAGES);I.pixelStorei(I.UNPACK_ROW_LENGTH,Nt.width),I.pixelStorei(I.UNPACK_IMAGE_HEIGHT,Nt.height),I.pixelStorei(I.UNPACK_SKIP_PIXELS,Ie),I.pixelStorei(I.UNPACK_SKIP_ROWS,ot),I.pixelStorei(I.UNPACK_SKIP_IMAGES,st),L.isDataTexture||L.isData3DTexture?I.texSubImage3D(Je,$,Ze,xt,Ot,De,Ve,Ye,nn,ht,Nt.data):K.isCompressedArrayTexture?I.compressedTexSubImage3D(Je,$,Ze,xt,Ot,De,Ve,Ye,nn,Nt.data):I.texSubImage3D(Je,$,Ze,xt,Ot,De,Ve,Ye,nn,ht,Nt),I.pixelStorei(I.UNPACK_ROW_LENGTH,At),I.pixelStorei(I.UNPACK_IMAGE_HEIGHT,Et),I.pixelStorei(I.UNPACK_SKIP_PIXELS,Hn),I.pixelStorei(I.UNPACK_SKIP_ROWS,Qi),I.pixelStorei(I.UNPACK_SKIP_IMAGES,rn),$===0&&K.generateMipmaps&&I.generateMipmap(Je),pe.unbindTexture()},this.initRenderTarget=function(L){Be.get(L).__webglFramebuffer===void 0&&We.setupRenderTarget(L)},this.initTexture=function(L){L.isCubeTexture?We.setTextureCube(L,0):L.isData3DTexture?We.setTexture3D(L,0):L.isDataArrayTexture||L.isCompressedArrayTexture?We.setTexture2DArray(L,0):We.setTexture2D(L,0),pe.unbindTexture()},this.resetState=function(){H=0,D=0,N=null,pe.reset(),ft.reset()},typeof __THREE_DEVTOOLS__<"u"&&__THREE_DEVTOOLS__.dispatchEvent(new CustomEvent("observe",{detail:this}))}get coordinateSystem(){return ji}get outputColorSpace(){return this._outputColorSpace}set outputColorSpace(e){this._outputColorSpace=e;const t=this.getContext();t.drawingBufferColorSpace=e===xd?"display-p3":"srgb",t.unpackColorSpace=Tt.workingColorSpace===Nu?"display-p3":"srgb"}}class bT extends cn{constructor(){super(),this.isScene=!0,this.type="Scene",this.background=null,this.environment=null,this.fog=null,this.backgroundBlurriness=0,this.backgroundIntensity=1,this.backgroundRotation=new Ai,this.environmentIntensity=1,this.environmentRotation=new Ai,this.overrideMaterial=null,typeof __THREE_DEVTOOLS__<"u"&&__THREE_DEVTOOLS__.dispatchEvent(new CustomEvent("observe",{detail:this}))}copy(e,t){return super.copy(e,t),e.background!==null&&(this.background=e.background.clone()),e.environment!==null&&(this.environment=e.environment.clone()),e.fog!==null&&(this.fog=e.fog.clone()),this.backgroundBlurriness=e.backgroundBlurriness,this.backgroundIntensity=e.backgroundIntensity,this.backgroundRotation.copy(e.backgroundRotation),this.environmentIntensity=e.environmentIntensity,this.environmentRotation.copy(e.environmentRotation),e.overrideMaterial!==null&&(this.overrideMaterial=e.overrideMaterial.clone()),this.matrixAutoUpdate=e.matrixAutoUpdate,this}toJSON(e){const t=super.toJSON(e);return this.fog!==null&&(t.object.fog=this.fog.toJSON()),this.backgroundBlurriness>0&&(t.object.backgroundBlurriness=this.backgroundBlurriness),this.backgroundIntensity!==1&&(t.object.backgroundIntensity=this.backgroundIntensity),t.object.backgroundRotation=this.backgroundRotation.toArray(),this.environmentIntensity!==1&&(t.object.environmentIntensity=this.environmentIntensity),t.object.environmentRotation=this.environmentRotation.toArray(),t}}class LT{constructor(e,t){this.isInterleavedBuffer=!0,this.array=e,this.stride=t,this.count=e!==void 0?e.length/t:0,this.usage=rd,this._updateRange={offset:0,count:-1},this.updateRanges=[],this.version=0,this.uuid=Yi()}onUploadCallback(){}set needsUpdate(e){e===!0&&this.version++}get updateRange(){return go("THREE.InterleavedBuffer: updateRange() is deprecated and will be removed in r169. Use addUpdateRange() instead."),this._updateRange}setUsage(e){return this.usage=e,this}addUpdateRange(e,t){this.updateRanges.push({start:e,count:t})}clearUpdateRanges(){this.updateRanges.length=0}copy(e){return this.array=new e.array.constructor(e.array),this.count=e.count,this.stride=e.stride,this.usage=e.usage,this}copyAt(e,t,i){e*=this.stride,i*=t.stride;for(let o=0,a=this.stride;o<a;o++)this.array[e+o]=t.array[i+o];return this}set(e,t=0){return this.array.set(e,t),this}clone(e){e.arrayBuffers===void 0&&(e.arrayBuffers={}),this.array.buffer._uuid===void 0&&(this.array.buffer._uuid=Yi()),e.arrayBuffers[this.array.buffer._uuid]===void 0&&(e.arrayBuffers[this.array.buffer._uuid]=this.array.slice(0).buffer);const t=new this.array.constructor(e.arrayBuffers[this.array.buffer._uuid]),i=new this.constructor(t,this.stride);return i.setUsage(this.usage),i}onUpload(e){return this.onUploadCallback=e,this}toJSON(e){return e.arrayBuffers===void 0&&(e.arrayBuffers={}),this.array.buffer._uuid===void 0&&(this.array.buffer._uuid=Yi()),e.arrayBuffers[this.array.buffer._uuid]===void 0&&(e.arrayBuffers[this.array.buffer._uuid]=Array.from(new Uint32Array(this.array.buffer))),{uuid:this.uuid,buffer:this.array.buffer._uuid,type:this.array.constructor.name,stride:this.stride}}}const Tn=new W;class Pu{constructor(e,t,i,o=!1){this.isInterleavedBufferAttribute=!0,this.name="",this.data=e,this.itemSize=t,this.offset=i,this.normalized=o}get count(){return this.data.count}get array(){return this.data.array}set needsUpdate(e){this.data.needsUpdate=e}applyMatrix4(e){for(let t=0,i=this.data.count;t<i;t++)Tn.fromBufferAttribute(this,t),Tn.applyMatrix4(e),this.setXYZ(t,Tn.x,Tn.y,Tn.z);return this}applyNormalMatrix(e){for(let t=0,i=this.count;t<i;t++)Tn.fromBufferAttribute(this,t),Tn.applyNormalMatrix(e),this.setXYZ(t,Tn.x,Tn.y,Tn.z);return this}transformDirection(e){for(let t=0,i=this.count;t<i;t++)Tn.fromBufferAttribute(this,t),Tn.transformDirection(e),this.setXYZ(t,Tn.x,Tn.y,Tn.z);return this}getComponent(e,t){let i=this.array[e*this.data.stride+this.offset+t];return this.normalized&&(i=Ti(i,this.array)),i}setComponent(e,t,i){return this.normalized&&(i=Rt(i,this.array)),this.data.array[e*this.data.stride+this.offset+t]=i,this}setX(e,t){return this.normalized&&(t=Rt(t,this.array)),this.data.array[e*this.data.stride+this.offset]=t,this}setY(e,t){return this.normalized&&(t=Rt(t,this.array)),this.data.array[e*this.data.stride+this.offset+1]=t,this}setZ(e,t){return this.normalized&&(t=Rt(t,this.array)),this.data.array[e*this.data.stride+this.offset+2]=t,this}setW(e,t){return this.normalized&&(t=Rt(t,this.array)),this.data.array[e*this.data.stride+this.offset+3]=t,this}getX(e){let t=this.data.array[e*this.data.stride+this.offset];return this.normalized&&(t=Ti(t,this.array)),t}getY(e){let t=this.data.array[e*this.data.stride+this.offset+1];return this.normalized&&(t=Ti(t,this.array)),t}getZ(e){let t=this.data.array[e*this.data.stride+this.offset+2];return this.normalized&&(t=Ti(t,this.array)),t}getW(e){let t=this.data.array[e*this.data.stride+this.offset+3];return this.normalized&&(t=Ti(t,this.array)),t}setXY(e,t,i){return e=e*this.data.stride+this.offset,this.normalized&&(t=Rt(t,this.array),i=Rt(i,this.array)),this.data.array[e+0]=t,this.data.array[e+1]=i,this}setXYZ(e,t,i,o){return e=e*this.data.stride+this.offset,this.normalized&&(t=Rt(t,this.array),i=Rt(i,this.array),o=Rt(o,this.array)),this.data.array[e+0]=t,this.data.array[e+1]=i,this.data.array[e+2]=o,this}setXYZW(e,t,i,o,a){return e=e*this.data.stride+this.offset,this.normalized&&(t=Rt(t,this.array),i=Rt(i,this.array),o=Rt(o,this.array),a=Rt(a,this.array)),this.data.array[e+0]=t,this.data.array[e+1]=i,this.data.array[e+2]=o,this.data.array[e+3]=a,this}clone(e){if(e===void 0){console.log("THREE.InterleavedBufferAttribute.clone(): Cloning an interleaved buffer attribute will de-interleave buffer data.");const t=[];for(let i=0;i<this.count;i++){const o=i*this.data.stride+this.offset;for(let a=0;a<this.itemSize;a++)t.push(this.data.array[o+a])}return new gi(new this.array.constructor(t),this.itemSize,this.normalized)}else return e.interleavedBuffers===void 0&&(e.interleavedBuffers={}),e.interleavedBuffers[this.data.uuid]===void 0&&(e.interleavedBuffers[this.data.uuid]=this.data.clone(e)),new Pu(e.interleavedBuffers[this.data.uuid],this.itemSize,this.offset,this.normalized)}toJSON(e){if(e===void 0){console.log("THREE.InterleavedBufferAttribute.toJSON(): Serializing an interleaved buffer attribute will de-interleave buffer data.");const t=[];for(let i=0;i<this.count;i++){const o=i*this.data.stride+this.offset;for(let a=0;a<this.itemSize;a++)t.push(this.data.array[o+a])}return{itemSize:this.itemSize,type:this.array.constructor.name,array:t,normalized:this.normalized}}else return e.interleavedBuffers===void 0&&(e.interleavedBuffers={}),e.interleavedBuffers[this.data.uuid]===void 0&&(e.interleavedBuffers[this.data.uuid]=this.data.toJSON(e)),{isInterleavedBufferAttribute:!0,itemSize:this.itemSize,data:this.data.uuid,offset:this.offset,normalized:this.normalized}}}class oo extends ds{constructor(e){super(),this.isSpriteMaterial=!0,this.type="SpriteMaterial",this.color=new yt(16777215),this.map=null,this.alphaMap=null,this.rotation=0,this.sizeAttenuation=!0,this.transparent=!0,this.fog=!0,this.setValues(e)}copy(e){return super.copy(e),this.color.copy(e.color),this.map=e.map,this.alphaMap=e.alphaMap,this.rotation=e.rotation,this.sizeAttenuation=e.sizeAttenuation,this.fog=e.fog,this}}let no;const fa=new W,io=new W,ro=new W,so=new Pe,ha=new Pe,H_=new kt,ru=new W,da=new W,su=new W,U0=new Pe,vh=new Pe,F0=new Pe;class pa extends cn{constructor(e=new oo){if(super(),this.isSprite=!0,this.type="Sprite",no===void 0){no=new un;const t=new Float32Array([-.5,-.5,0,0,0,.5,-.5,0,1,0,.5,.5,0,1,1,-.5,.5,0,0,1]),i=new LT(t,5);no.setIndex([0,1,2,0,2,3]),no.setAttribute("position",new Pu(i,3,0,!1)),no.setAttribute("uv",new Pu(i,2,3,!1))}this.geometry=no,this.material=e,this.center=new Pe(.5,.5)}raycast(e,t){e.camera===null&&console.error('THREE.Sprite: "Raycaster.camera" needs to be set in order to raycast against sprites.'),io.setFromMatrixScale(this.matrixWorld),H_.copy(e.camera.matrixWorld),this.modelViewMatrix.multiplyMatrices(e.camera.matrixWorldInverse,this.matrixWorld),ro.setFromMatrixPosition(this.modelViewMatrix),e.camera.isPerspectiveCamera&&this.material.sizeAttenuation===!1&&io.multiplyScalar(-ro.z);const i=this.material.rotation;let o,a;i!==0&&(a=Math.cos(i),o=Math.sin(i));const u=this.center;ou(ru.set(-.5,-.5,0),ro,u,io,o,a),ou(da.set(.5,-.5,0),ro,u,io,o,a),ou(su.set(.5,.5,0),ro,u,io,o,a),U0.set(0,0),vh.set(1,0),F0.set(1,1);let c=e.ray.intersectTriangle(ru,da,su,!1,fa);if(c===null&&(ou(da.set(-.5,.5,0),ro,u,io,o,a),vh.set(0,1),c=e.ray.intersectTriangle(ru,su,da,!1,fa),c===null))return;const h=e.ray.origin.distanceTo(fa);h<e.near||h>e.far||t.push({distance:h,point:fa.clone(),uv:ii.getInterpolation(fa,ru,da,su,U0,vh,F0,new Pe),face:null,object:this})}copy(e,t){return super.copy(e,t),e.center!==void 0&&this.center.copy(e.center),this.material=e.material,this}}function ou(s,e,t,i,o,a){so.subVectors(s,t).addScalar(.5).multiply(i),o!==void 0?(ha.x=a*so.x-o*so.y,ha.y=o*so.x+a*so.y):ha.copy(so),s.copy(e),s.x+=ha.x,s.y+=ha.y,s.applyMatrix4(H_)}class Pr extends ds{constructor(e){super(),this.isLineBasicMaterial=!0,this.type="LineBasicMaterial",this.color=new yt(16777215),this.map=null,this.linewidth=1,this.linecap="round",this.linejoin="round",this.fog=!0,this.setValues(e)}copy(e){return super.copy(e),this.color.copy(e.color),this.map=e.map,this.linewidth=e.linewidth,this.linecap=e.linecap,this.linejoin=e.linejoin,this.fog=e.fog,this}}const bu=new W,Lu=new W,O0=new kt,ma=new Sd,au=new Iu,yh=new W,B0=new W;class ao extends cn{constructor(e=new un,t=new Pr){super(),this.isLine=!0,this.type="Line",this.geometry=e,this.material=t,this.updateMorphTargets()}copy(e,t){return super.copy(e,t),this.material=Array.isArray(e.material)?e.material.slice():e.material,this.geometry=e.geometry,this}computeLineDistances(){const e=this.geometry;if(e.index===null){const t=e.attributes.position,i=[0];for(let o=1,a=t.count;o<a;o++)bu.fromBufferAttribute(t,o-1),Lu.fromBufferAttribute(t,o),i[o]=i[o-1],i[o]+=bu.distanceTo(Lu);e.setAttribute("lineDistance",new gn(i,1))}else console.warn("THREE.Line.computeLineDistances(): Computation only possible with non-indexed BufferGeometry.");return this}raycast(e,t){const i=this.geometry,o=this.matrixWorld,a=e.params.Line.threshold,u=i.drawRange;if(i.boundingSphere===null&&i.computeBoundingSphere(),au.copy(i.boundingSphere),au.applyMatrix4(o),au.radius+=a,e.ray.intersectsSphere(au)===!1)return;O0.copy(o).invert(),ma.copy(e.ray).applyMatrix4(O0);const c=a/((this.scale.x+this.scale.y+this.scale.z)/3),h=c*c,d=this.isLineSegments?2:1,m=i.index,g=i.attributes.position;if(m!==null){const S=Math.max(0,u.start),M=Math.min(m.count,u.start+u.count);for(let E=S,y=M-1;E<y;E+=d){const x=m.getX(E),b=m.getX(E+1),T=lu(this,e,ma,h,x,b);T&&t.push(T)}if(this.isLineLoop){const E=m.getX(M-1),y=m.getX(S),x=lu(this,e,ma,h,E,y);x&&t.push(x)}}else{const S=Math.max(0,u.start),M=Math.min(g.count,u.start+u.count);for(let E=S,y=M-1;E<y;E+=d){const x=lu(this,e,ma,h,E,E+1);x&&t.push(x)}if(this.isLineLoop){const E=lu(this,e,ma,h,M-1,S);E&&t.push(E)}}}updateMorphTargets(){const t=this.geometry.morphAttributes,i=Object.keys(t);if(i.length>0){const o=t[i[0]];if(o!==void 0){this.morphTargetInfluences=[],this.morphTargetDictionary={};for(let a=0,u=o.length;a<u;a++){const c=o[a].name||String(a);this.morphTargetInfluences.push(0),this.morphTargetDictionary[c]=a}}}}}function lu(s,e,t,i,o,a){const u=s.geometry.attributes.position;if(bu.fromBufferAttribute(u,o),Lu.fromBufferAttribute(u,a),t.distanceSqToSegment(bu,Lu,yh,B0)>i)return;yh.applyMatrix4(s.matrixWorld);const h=e.ray.origin.distanceTo(yh);if(!(h<e.near||h>e.far))return{distance:h,point:B0.clone().applyMatrix4(s.matrixWorld),index:o,face:null,faceIndex:null,object:s}}const z0=new W,k0=new W;class V_ extends ao{constructor(e,t){super(e,t),this.isLineSegments=!0,this.type="LineSegments"}computeLineDistances(){const e=this.geometry;if(e.index===null){const t=e.attributes.position,i=[];for(let o=0,a=t.count;o<a;o+=2)z0.fromBufferAttribute(t,o),k0.fromBufferAttribute(t,o+1),i[o]=o===0?0:i[o-1],i[o+1]=i[o]+z0.distanceTo(k0);e.setAttribute("lineDistance",new gn(i,1))}else console.warn("THREE.LineSegments.computeLineDistances(): Computation only possible with non-indexed BufferGeometry.");return this}}class lo extends Rn{constructor(e,t,i,o,a,u,c,h,d){super(e,t,i,o,a,u,c,h,d),this.isCanvasTexture=!0,this.needsUpdate=!0}}class Ci{constructor(){this.type="Curve",this.arcLengthDivisions=200}getPoint(){return console.warn("THREE.Curve: .getPoint() not implemented."),null}getPointAt(e,t){const i=this.getUtoTmapping(e);return this.getPoint(i,t)}getPoints(e=5){const t=[];for(let i=0;i<=e;i++)t.push(this.getPoint(i/e));return t}getSpacedPoints(e=5){const t=[];for(let i=0;i<=e;i++)t.push(this.getPointAt(i/e));return t}getLength(){const e=this.getLengths();return e[e.length-1]}getLengths(e=this.arcLengthDivisions){if(this.cacheArcLengths&&this.cacheArcLengths.length===e+1&&!this.needsUpdate)return this.cacheArcLengths;this.needsUpdate=!1;const t=[];let i,o=this.getPoint(0),a=0;t.push(0);for(let u=1;u<=e;u++)i=this.getPoint(u/e),a+=i.distanceTo(o),t.push(a),o=i;return this.cacheArcLengths=t,t}updateArcLengths(){this.needsUpdate=!0,this.getLengths()}getUtoTmapping(e,t){const i=this.getLengths();let o=0;const a=i.length;let u;t?u=t:u=e*i[a-1];let c=0,h=a-1,d;for(;c<=h;)if(o=Math.floor(c+(h-c)/2),d=i[o]-u,d<0)c=o+1;else if(d>0)h=o-1;else{h=o;break}if(o=h,i[o]===u)return o/(a-1);const m=i[o],g=i[o+1]-m,S=(u-m)/g;return(o+S)/(a-1)}getTangent(e,t){let o=e-1e-4,a=e+1e-4;o<0&&(o=0),a>1&&(a=1);const u=this.getPoint(o),c=this.getPoint(a),h=t||(u.isVector2?new Pe:new W);return h.copy(c).sub(u).normalize(),h}getTangentAt(e,t){const i=this.getUtoTmapping(e);return this.getTangent(i,t)}computeFrenetFrames(e,t){const i=new W,o=[],a=[],u=[],c=new W,h=new kt;for(let S=0;S<=e;S++){const M=S/e;o[S]=this.getTangentAt(M,new W)}a[0]=new W,u[0]=new W;let d=Number.MAX_VALUE;const m=Math.abs(o[0].x),_=Math.abs(o[0].y),g=Math.abs(o[0].z);m<=d&&(d=m,i.set(1,0,0)),_<=d&&(d=_,i.set(0,1,0)),g<=d&&i.set(0,0,1),c.crossVectors(o[0],i).normalize(),a[0].crossVectors(o[0],c),u[0].crossVectors(o[0],a[0]);for(let S=1;S<=e;S++){if(a[S]=a[S-1].clone(),u[S]=u[S-1].clone(),c.crossVectors(o[S-1],o[S]),c.length()>Number.EPSILON){c.normalize();const M=Math.acos(pn(o[S-1].dot(o[S]),-1,1));a[S].applyMatrix4(h.makeRotationAxis(c,M))}u[S].crossVectors(o[S],a[S])}if(t===!0){let S=Math.acos(pn(a[0].dot(a[e]),-1,1));S/=e,o[0].dot(c.crossVectors(a[0],a[e]))>0&&(S=-S);for(let M=1;M<=e;M++)a[M].applyMatrix4(h.makeRotationAxis(o[M],S*M)),u[M].crossVectors(o[M],a[M])}return{tangents:o,normals:a,binormals:u}}clone(){return new this.constructor().copy(this)}copy(e){return this.arcLengthDivisions=e.arcLengthDivisions,this}toJSON(){const e={metadata:{version:4.6,type:"Curve",generator:"Curve.toJSON"}};return e.arcLengthDivisions=this.arcLengthDivisions,e.type=this.type,e}fromJSON(e){return this.arcLengthDivisions=e.arcLengthDivisions,this}}class wd extends Ci{constructor(e=0,t=0,i=1,o=1,a=0,u=Math.PI*2,c=!1,h=0){super(),this.isEllipseCurve=!0,this.type="EllipseCurve",this.aX=e,this.aY=t,this.xRadius=i,this.yRadius=o,this.aStartAngle=a,this.aEndAngle=u,this.aClockwise=c,this.aRotation=h}getPoint(e,t=new Pe){const i=t,o=Math.PI*2;let a=this.aEndAngle-this.aStartAngle;const u=Math.abs(a)<Number.EPSILON;for(;a<0;)a+=o;for(;a>o;)a-=o;a<Number.EPSILON&&(u?a=0:a=o),this.aClockwise===!0&&!u&&(a===o?a=-o:a=a-o);const c=this.aStartAngle+e*a;let h=this.aX+this.xRadius*Math.cos(c),d=this.aY+this.yRadius*Math.sin(c);if(this.aRotation!==0){const m=Math.cos(this.aRotation),_=Math.sin(this.aRotation),g=h-this.aX,S=d-this.aY;h=g*m-S*_+this.aX,d=g*_+S*m+this.aY}return i.set(h,d)}copy(e){return super.copy(e),this.aX=e.aX,this.aY=e.aY,this.xRadius=e.xRadius,this.yRadius=e.yRadius,this.aStartAngle=e.aStartAngle,this.aEndAngle=e.aEndAngle,this.aClockwise=e.aClockwise,this.aRotation=e.aRotation,this}toJSON(){const e=super.toJSON();return e.aX=this.aX,e.aY=this.aY,e.xRadius=this.xRadius,e.yRadius=this.yRadius,e.aStartAngle=this.aStartAngle,e.aEndAngle=this.aEndAngle,e.aClockwise=this.aClockwise,e.aRotation=this.aRotation,e}fromJSON(e){return super.fromJSON(e),this.aX=e.aX,this.aY=e.aY,this.xRadius=e.xRadius,this.yRadius=e.yRadius,this.aStartAngle=e.aStartAngle,this.aEndAngle=e.aEndAngle,this.aClockwise=e.aClockwise,this.aRotation=e.aRotation,this}}class DT extends wd{constructor(e,t,i,o,a,u){super(e,t,i,i,o,a,u),this.isArcCurve=!0,this.type="ArcCurve"}}function Td(){let s=0,e=0,t=0,i=0;function o(a,u,c,h){s=a,e=c,t=-3*a+3*u-2*c-h,i=2*a-2*u+c+h}return{initCatmullRom:function(a,u,c,h,d){o(u,c,d*(c-a),d*(h-u))},initNonuniformCatmullRom:function(a,u,c,h,d,m,_){let g=(u-a)/d-(c-a)/(d+m)+(c-u)/m,S=(c-u)/m-(h-u)/(m+_)+(h-c)/_;g*=m,S*=m,o(u,c,g,S)},calc:function(a){const u=a*a,c=u*a;return s+e*a+t*u+i*c}}}const uu=new W,xh=new Td,Sh=new Td,Mh=new Td;class NT extends Ci{constructor(e=[],t=!1,i="centripetal",o=.5){super(),this.isCatmullRomCurve3=!0,this.type="CatmullRomCurve3",this.points=e,this.closed=t,this.curveType=i,this.tension=o}getPoint(e,t=new W){const i=t,o=this.points,a=o.length,u=(a-(this.closed?0:1))*e;let c=Math.floor(u),h=u-c;this.closed?c+=c>0?0:(Math.floor(Math.abs(c)/a)+1)*a:h===0&&c===a-1&&(c=a-2,h=1);let d,m;this.closed||c>0?d=o[(c-1)%a]:(uu.subVectors(o[0],o[1]).add(o[0]),d=uu);const _=o[c%a],g=o[(c+1)%a];if(this.closed||c+2<a?m=o[(c+2)%a]:(uu.subVectors(o[a-1],o[a-2]).add(o[a-1]),m=uu),this.curveType==="centripetal"||this.curveType==="chordal"){const S=this.curveType==="chordal"?.5:.25;let M=Math.pow(d.distanceToSquared(_),S),E=Math.pow(_.distanceToSquared(g),S),y=Math.pow(g.distanceToSquared(m),S);E<1e-4&&(E=1),M<1e-4&&(M=E),y<1e-4&&(y=E),xh.initNonuniformCatmullRom(d.x,_.x,g.x,m.x,M,E,y),Sh.initNonuniformCatmullRom(d.y,_.y,g.y,m.y,M,E,y),Mh.initNonuniformCatmullRom(d.z,_.z,g.z,m.z,M,E,y)}else this.curveType==="catmullrom"&&(xh.initCatmullRom(d.x,_.x,g.x,m.x,this.tension),Sh.initCatmullRom(d.y,_.y,g.y,m.y,this.tension),Mh.initCatmullRom(d.z,_.z,g.z,m.z,this.tension));return i.set(xh.calc(h),Sh.calc(h),Mh.calc(h)),i}copy(e){super.copy(e),this.points=[];for(let t=0,i=e.points.length;t<i;t++){const o=e.points[t];this.points.push(o.clone())}return this.closed=e.closed,this.curveType=e.curveType,this.tension=e.tension,this}toJSON(){const e=super.toJSON();e.points=[];for(let t=0,i=this.points.length;t<i;t++){const o=this.points[t];e.points.push(o.toArray())}return e.closed=this.closed,e.curveType=this.curveType,e.tension=this.tension,e}fromJSON(e){super.fromJSON(e),this.points=[];for(let t=0,i=e.points.length;t<i;t++){const o=e.points[t];this.points.push(new W().fromArray(o))}return this.closed=e.closed,this.curveType=e.curveType,this.tension=e.tension,this}}function H0(s,e,t,i,o){const a=(i-e)*.5,u=(o-t)*.5,c=s*s,h=s*c;return(2*t-2*i+a+u)*h+(-3*t+3*i-2*a-u)*c+a*s+t}function IT(s,e){const t=1-s;return t*t*e}function UT(s,e){return 2*(1-s)*s*e}function FT(s,e){return s*s*e}function va(s,e,t,i){return IT(s,e)+UT(s,t)+FT(s,i)}function OT(s,e){const t=1-s;return t*t*t*e}function BT(s,e){const t=1-s;return 3*t*t*s*e}function zT(s,e){return 3*(1-s)*s*s*e}function kT(s,e){return s*s*s*e}function ya(s,e,t,i,o){return OT(s,e)+BT(s,t)+zT(s,i)+kT(s,o)}class G_ extends Ci{constructor(e=new Pe,t=new Pe,i=new Pe,o=new Pe){super(),this.isCubicBezierCurve=!0,this.type="CubicBezierCurve",this.v0=e,this.v1=t,this.v2=i,this.v3=o}getPoint(e,t=new Pe){const i=t,o=this.v0,a=this.v1,u=this.v2,c=this.v3;return i.set(ya(e,o.x,a.x,u.x,c.x),ya(e,o.y,a.y,u.y,c.y)),i}copy(e){return super.copy(e),this.v0.copy(e.v0),this.v1.copy(e.v1),this.v2.copy(e.v2),this.v3.copy(e.v3),this}toJSON(){const e=super.toJSON();return e.v0=this.v0.toArray(),e.v1=this.v1.toArray(),e.v2=this.v2.toArray(),e.v3=this.v3.toArray(),e}fromJSON(e){return super.fromJSON(e),this.v0.fromArray(e.v0),this.v1.fromArray(e.v1),this.v2.fromArray(e.v2),this.v3.fromArray(e.v3),this}}class HT extends Ci{constructor(e=new W,t=new W,i=new W,o=new W){super(),this.isCubicBezierCurve3=!0,this.type="CubicBezierCurve3",this.v0=e,this.v1=t,this.v2=i,this.v3=o}getPoint(e,t=new W){const i=t,o=this.v0,a=this.v1,u=this.v2,c=this.v3;return i.set(ya(e,o.x,a.x,u.x,c.x),ya(e,o.y,a.y,u.y,c.y),ya(e,o.z,a.z,u.z,c.z)),i}copy(e){return super.copy(e),this.v0.copy(e.v0),this.v1.copy(e.v1),this.v2.copy(e.v2),this.v3.copy(e.v3),this}toJSON(){const e=super.toJSON();return e.v0=this.v0.toArray(),e.v1=this.v1.toArray(),e.v2=this.v2.toArray(),e.v3=this.v3.toArray(),e}fromJSON(e){return super.fromJSON(e),this.v0.fromArray(e.v0),this.v1.fromArray(e.v1),this.v2.fromArray(e.v2),this.v3.fromArray(e.v3),this}}class W_ extends Ci{constructor(e=new Pe,t=new Pe){super(),this.isLineCurve=!0,this.type="LineCurve",this.v1=e,this.v2=t}getPoint(e,t=new Pe){const i=t;return e===1?i.copy(this.v2):(i.copy(this.v2).sub(this.v1),i.multiplyScalar(e).add(this.v1)),i}getPointAt(e,t){return this.getPoint(e,t)}getTangent(e,t=new Pe){return t.subVectors(this.v2,this.v1).normalize()}getTangentAt(e,t){return this.getTangent(e,t)}copy(e){return super.copy(e),this.v1.copy(e.v1),this.v2.copy(e.v2),this}toJSON(){const e=super.toJSON();return e.v1=this.v1.toArray(),e.v2=this.v2.toArray(),e}fromJSON(e){return super.fromJSON(e),this.v1.fromArray(e.v1),this.v2.fromArray(e.v2),this}}class VT extends Ci{constructor(e=new W,t=new W){super(),this.isLineCurve3=!0,this.type="LineCurve3",this.v1=e,this.v2=t}getPoint(e,t=new W){const i=t;return e===1?i.copy(this.v2):(i.copy(this.v2).sub(this.v1),i.multiplyScalar(e).add(this.v1)),i}getPointAt(e,t){return this.getPoint(e,t)}getTangent(e,t=new W){return t.subVectors(this.v2,this.v1).normalize()}getTangentAt(e,t){return this.getTangent(e,t)}copy(e){return super.copy(e),this.v1.copy(e.v1),this.v2.copy(e.v2),this}toJSON(){const e=super.toJSON();return e.v1=this.v1.toArray(),e.v2=this.v2.toArray(),e}fromJSON(e){return super.fromJSON(e),this.v1.fromArray(e.v1),this.v2.fromArray(e.v2),this}}class X_ extends Ci{constructor(e=new Pe,t=new Pe,i=new Pe){super(),this.isQuadraticBezierCurve=!0,this.type="QuadraticBezierCurve",this.v0=e,this.v1=t,this.v2=i}getPoint(e,t=new Pe){const i=t,o=this.v0,a=this.v1,u=this.v2;return i.set(va(e,o.x,a.x,u.x),va(e,o.y,a.y,u.y)),i}copy(e){return super.copy(e),this.v0.copy(e.v0),this.v1.copy(e.v1),this.v2.copy(e.v2),this}toJSON(){const e=super.toJSON();return e.v0=this.v0.toArray(),e.v1=this.v1.toArray(),e.v2=this.v2.toArray(),e}fromJSON(e){return super.fromJSON(e),this.v0.fromArray(e.v0),this.v1.fromArray(e.v1),this.v2.fromArray(e.v2),this}}class GT extends Ci{constructor(e=new W,t=new W,i=new W){super(),this.isQuadraticBezierCurve3=!0,this.type="QuadraticBezierCurve3",this.v0=e,this.v1=t,this.v2=i}getPoint(e,t=new W){const i=t,o=this.v0,a=this.v1,u=this.v2;return i.set(va(e,o.x,a.x,u.x),va(e,o.y,a.y,u.y),va(e,o.z,a.z,u.z)),i}copy(e){return super.copy(e),this.v0.copy(e.v0),this.v1.copy(e.v1),this.v2.copy(e.v2),this}toJSON(){const e=super.toJSON();return e.v0=this.v0.toArray(),e.v1=this.v1.toArray(),e.v2=this.v2.toArray(),e}fromJSON(e){return super.fromJSON(e),this.v0.fromArray(e.v0),this.v1.fromArray(e.v1),this.v2.fromArray(e.v2),this}}class j_ extends Ci{constructor(e=[]){super(),this.isSplineCurve=!0,this.type="SplineCurve",this.points=e}getPoint(e,t=new Pe){const i=t,o=this.points,a=(o.length-1)*e,u=Math.floor(a),c=a-u,h=o[u===0?u:u-1],d=o[u],m=o[u>o.length-2?o.length-1:u+1],_=o[u>o.length-3?o.length-1:u+2];return i.set(H0(c,h.x,d.x,m.x,_.x),H0(c,h.y,d.y,m.y,_.y)),i}copy(e){super.copy(e),this.points=[];for(let t=0,i=e.points.length;t<i;t++){const o=e.points[t];this.points.push(o.clone())}return this}toJSON(){const e=super.toJSON();e.points=[];for(let t=0,i=this.points.length;t<i;t++){const o=this.points[t];e.points.push(o.toArray())}return e}fromJSON(e){super.fromJSON(e),this.points=[];for(let t=0,i=e.points.length;t<i;t++){const o=e.points[t];this.points.push(new Pe().fromArray(o))}return this}}var ad=Object.freeze({__proto__:null,ArcCurve:DT,CatmullRomCurve3:NT,CubicBezierCurve:G_,CubicBezierCurve3:HT,EllipseCurve:wd,LineCurve:W_,LineCurve3:VT,QuadraticBezierCurve:X_,QuadraticBezierCurve3:GT,SplineCurve:j_});class WT extends Ci{constructor(){super(),this.type="CurvePath",this.curves=[],this.autoClose=!1}add(e){this.curves.push(e)}closePath(){const e=this.curves[0].getPoint(0),t=this.curves[this.curves.length-1].getPoint(1);if(!e.equals(t)){const i=e.isVector2===!0?"LineCurve":"LineCurve3";this.curves.push(new ad[i](t,e))}return this}getPoint(e,t){const i=e*this.getLength(),o=this.getCurveLengths();let a=0;for(;a<o.length;){if(o[a]>=i){const u=o[a]-i,c=this.curves[a],h=c.getLength(),d=h===0?0:1-u/h;return c.getPointAt(d,t)}a++}return null}getLength(){const e=this.getCurveLengths();return e[e.length-1]}updateArcLengths(){this.needsUpdate=!0,this.cacheLengths=null,this.getCurveLengths()}getCurveLengths(){if(this.cacheLengths&&this.cacheLengths.length===this.curves.length)return this.cacheLengths;const e=[];let t=0;for(let i=0,o=this.curves.length;i<o;i++)t+=this.curves[i].getLength(),e.push(t);return this.cacheLengths=e,e}getSpacedPoints(e=40){const t=[];for(let i=0;i<=e;i++)t.push(this.getPoint(i/e));return this.autoClose&&t.push(t[0]),t}getPoints(e=12){const t=[];let i;for(let o=0,a=this.curves;o<a.length;o++){const u=a[o],c=u.isEllipseCurve?e*2:u.isLineCurve||u.isLineCurve3?1:u.isSplineCurve?e*u.points.length:e,h=u.getPoints(c);for(let d=0;d<h.length;d++){const m=h[d];i&&i.equals(m)||(t.push(m),i=m)}}return this.autoClose&&t.length>1&&!t[t.length-1].equals(t[0])&&t.push(t[0]),t}copy(e){super.copy(e),this.curves=[];for(let t=0,i=e.curves.length;t<i;t++){const o=e.curves[t];this.curves.push(o.clone())}return this.autoClose=e.autoClose,this}toJSON(){const e=super.toJSON();e.autoClose=this.autoClose,e.curves=[];for(let t=0,i=this.curves.length;t<i;t++){const o=this.curves[t];e.curves.push(o.toJSON())}return e}fromJSON(e){super.fromJSON(e),this.autoClose=e.autoClose,this.curves=[];for(let t=0,i=e.curves.length;t<i;t++){const o=e.curves[t];this.curves.push(new ad[o.type]().fromJSON(o))}return this}}class V0 extends WT{constructor(e){super(),this.type="Path",this.currentPoint=new Pe,e&&this.setFromPoints(e)}setFromPoints(e){this.moveTo(e[0].x,e[0].y);for(let t=1,i=e.length;t<i;t++)this.lineTo(e[t].x,e[t].y);return this}moveTo(e,t){return this.currentPoint.set(e,t),this}lineTo(e,t){const i=new W_(this.currentPoint.clone(),new Pe(e,t));return this.curves.push(i),this.currentPoint.set(e,t),this}quadraticCurveTo(e,t,i,o){const a=new X_(this.currentPoint.clone(),new Pe(e,t),new Pe(i,o));return this.curves.push(a),this.currentPoint.set(i,o),this}bezierCurveTo(e,t,i,o,a,u){const c=new G_(this.currentPoint.clone(),new Pe(e,t),new Pe(i,o),new Pe(a,u));return this.curves.push(c),this.currentPoint.set(a,u),this}splineThru(e){const t=[this.currentPoint.clone()].concat(e),i=new j_(t);return this.curves.push(i),this.currentPoint.copy(e[e.length-1]),this}arc(e,t,i,o,a,u){const c=this.currentPoint.x,h=this.currentPoint.y;return this.absarc(e+c,t+h,i,o,a,u),this}absarc(e,t,i,o,a,u){return this.absellipse(e,t,i,i,o,a,u),this}ellipse(e,t,i,o,a,u,c,h){const d=this.currentPoint.x,m=this.currentPoint.y;return this.absellipse(e+d,t+m,i,o,a,u,c,h),this}absellipse(e,t,i,o,a,u,c,h){const d=new wd(e,t,i,o,a,u,c,h);if(this.curves.length>0){const _=d.getPoint(0);_.equals(this.currentPoint)||this.lineTo(_.x,_.y)}this.curves.push(d);const m=d.getPoint(1);return this.currentPoint.copy(m),this}copy(e){return super.copy(e),this.currentPoint.copy(e.currentPoint),this}toJSON(){const e=super.toJSON();return e.currentPoint=this.currentPoint.toArray(),e}fromJSON(e){return super.fromJSON(e),this.currentPoint.fromArray(e.currentPoint),this}}class vo extends un{constructor(e=1,t=1,i=1,o=32,a=1,u=!1,c=0,h=Math.PI*2){super(),this.type="CylinderGeometry",this.parameters={radiusTop:e,radiusBottom:t,height:i,radialSegments:o,heightSegments:a,openEnded:u,thetaStart:c,thetaLength:h};const d=this;o=Math.floor(o),a=Math.floor(a);const m=[],_=[],g=[],S=[];let M=0;const E=[],y=i/2;let x=0;b(),u===!1&&(e>0&&T(!0),t>0&&T(!1)),this.setIndex(m),this.setAttribute("position",new gn(_,3)),this.setAttribute("normal",new gn(g,3)),this.setAttribute("uv",new gn(S,2));function b(){const P=new W,H=new W;let D=0;const N=(t-e)/i;for(let z=0;z<=a;z++){const C=[],w=z/a,U=w*(t-e)+e;for(let X=0;X<=o;X++){const k=X/o,Y=k*h+c,ae=Math.sin(Y),te=Math.cos(Y);H.x=U*ae,H.y=-w*i+y,H.z=U*te,_.push(H.x,H.y,H.z),P.set(ae,N,te).normalize(),g.push(P.x,P.y,P.z),S.push(k,1-w),C.push(M++)}E.push(C)}for(let z=0;z<o;z++)for(let C=0;C<a;C++){const w=E[C][z],U=E[C+1][z],X=E[C+1][z+1],k=E[C][z+1];m.push(w,U,k),m.push(U,X,k),D+=6}d.addGroup(x,D,0),x+=D}function T(P){const H=M,D=new Pe,N=new W;let z=0;const C=P===!0?e:t,w=P===!0?1:-1;for(let X=1;X<=o;X++)_.push(0,y*w,0),g.push(0,w,0),S.push(.5,.5),M++;const U=M;for(let X=0;X<=o;X++){const Y=X/o*h+c,ae=Math.cos(Y),te=Math.sin(Y);N.x=C*te,N.y=y*w,N.z=C*ae,_.push(N.x,N.y,N.z),g.push(0,w,0),D.x=ae*.5+.5,D.y=te*.5*w+.5,S.push(D.x,D.y),M++}for(let X=0;X<o;X++){const k=H+X,Y=U+X;P===!0?m.push(Y,Y+1,k):m.push(Y+1,Y,k),z+=3}d.addGroup(x,z,P===!0?1:2),x+=z}}copy(e){return super.copy(e),this.parameters=Object.assign({},e.parameters),this}static fromJSON(e){return new vo(e.radiusTop,e.radiusBottom,e.height,e.radialSegments,e.heightSegments,e.openEnded,e.thetaStart,e.thetaLength)}}class Ad extends vo{constructor(e=1,t=1,i=32,o=1,a=!1,u=0,c=Math.PI*2){super(0,e,t,i,o,a,u,c),this.type="ConeGeometry",this.parameters={radius:e,height:t,radialSegments:i,heightSegments:o,openEnded:a,thetaStart:u,thetaLength:c}}static fromJSON(e){return new Ad(e.radius,e.height,e.radialSegments,e.heightSegments,e.openEnded,e.thetaStart,e.thetaLength)}}const cu=new W,fu=new W,Eh=new W,hu=new ii;class XT extends un{constructor(e=null,t=1){if(super(),this.type="EdgesGeometry",this.parameters={geometry:e,thresholdAngle:t},e!==null){const o=Math.pow(10,4),a=Math.cos(_a*t),u=e.getIndex(),c=e.getAttribute("position"),h=u?u.count:c.count,d=[0,0,0],m=["a","b","c"],_=new Array(3),g={},S=[];for(let M=0;M<h;M+=3){u?(d[0]=u.getX(M),d[1]=u.getX(M+1),d[2]=u.getX(M+2)):(d[0]=M,d[1]=M+1,d[2]=M+2);const{a:E,b:y,c:x}=hu;if(E.fromBufferAttribute(c,d[0]),y.fromBufferAttribute(c,d[1]),x.fromBufferAttribute(c,d[2]),hu.getNormal(Eh),_[0]=`${Math.round(E.x*o)},${Math.round(E.y*o)},${Math.round(E.z*o)}`,_[1]=`${Math.round(y.x*o)},${Math.round(y.y*o)},${Math.round(y.z*o)}`,_[2]=`${Math.round(x.x*o)},${Math.round(x.y*o)},${Math.round(x.z*o)}`,!(_[0]===_[1]||_[1]===_[2]||_[2]===_[0]))for(let b=0;b<3;b++){const T=(b+1)%3,P=_[b],H=_[T],D=hu[m[b]],N=hu[m[T]],z=`${P}_${H}`,C=`${H}_${P}`;C in g&&g[C]?(Eh.dot(g[C].normal)<=a&&(S.push(D.x,D.y,D.z),S.push(N.x,N.y,N.z)),g[C]=null):z in g||(g[z]={index0:d[b],index1:d[T],normal:Eh.clone()})}}for(const M in g)if(g[M]){const{index0:E,index1:y}=g[M];cu.fromBufferAttribute(c,E),fu.fromBufferAttribute(c,y),S.push(cu.x,cu.y,cu.z),S.push(fu.x,fu.y,fu.z)}this.setAttribute("position",new gn(S,3))}}copy(e){return super.copy(e),this.parameters=Object.assign({},e.parameters),this}}class q_ extends V0{constructor(e){super(e),this.uuid=Yi(),this.type="Shape",this.holes=[]}getPointsHoles(e){const t=[];for(let i=0,o=this.holes.length;i<o;i++)t[i]=this.holes[i].getPoints(e);return t}extractPoints(e){return{shape:this.getPoints(e),holes:this.getPointsHoles(e)}}copy(e){super.copy(e),this.holes=[];for(let t=0,i=e.holes.length;t<i;t++){const o=e.holes[t];this.holes.push(o.clone())}return this}toJSON(){const e=super.toJSON();e.uuid=this.uuid,e.holes=[];for(let t=0,i=this.holes.length;t<i;t++){const o=this.holes[t];e.holes.push(o.toJSON())}return e}fromJSON(e){super.fromJSON(e),this.uuid=e.uuid,this.holes=[];for(let t=0,i=e.holes.length;t<i;t++){const o=e.holes[t];this.holes.push(new V0().fromJSON(o))}return this}}const jT={triangulate:function(s,e,t=2){const i=e&&e.length,o=i?e[0]*t:s.length;let a=Y_(s,0,o,t,!0);const u=[];if(!a||a.next===a.prev)return u;let c,h,d,m,_,g,S;if(i&&(a=ZT(s,e,a,t)),s.length>80*t){c=d=s[0],h=m=s[1];for(let M=t;M<o;M+=t)_=s[M],g=s[M+1],_<c&&(c=_),g<h&&(h=g),_>d&&(d=_),g>m&&(m=g);S=Math.max(d-c,m-h),S=S!==0?32767/S:0}return Ma(a,u,t,c,h,S,0),u}};function Y_(s,e,t,i,o){let a,u;if(o===lA(s,e,t,i)>0)for(a=e;a<t;a+=i)u=G0(a,s[a],s[a+1],u);else for(a=t-i;a>=e;a-=i)u=G0(a,s[a],s[a+1],u);return u&&Fu(u,u.next)&&(wa(u),u=u.next),u}function us(s,e){if(!s)return s;e||(e=s);let t=s,i;do if(i=!1,!t.steiner&&(Fu(t,t.next)||Wt(t.prev,t,t.next)===0)){if(wa(t),t=e=t.prev,t===t.next)break;i=!0}else t=t.next;while(i||t!==e);return e}function Ma(s,e,t,i,o,a,u){if(!s)return;!u&&a&&nA(s,i,o,a);let c=s,h,d;for(;s.prev!==s.next;){if(h=s.prev,d=s.next,a?YT(s,i,o,a):qT(s)){e.push(h.i/t|0),e.push(s.i/t|0),e.push(d.i/t|0),wa(s),s=d.next,c=d.next;continue}if(s=d,s===c){u?u===1?(s=KT(us(s),e,t),Ma(s,e,t,i,o,a,2)):u===2&&$T(s,e,t,i,o,a):Ma(us(s),e,t,i,o,a,1);break}}}function qT(s){const e=s.prev,t=s,i=s.next;if(Wt(e,t,i)>=0)return!1;const o=e.x,a=t.x,u=i.x,c=e.y,h=t.y,d=i.y,m=o<a?o<u?o:u:a<u?a:u,_=c<h?c<d?c:d:h<d?h:d,g=o>a?o>u?o:u:a>u?a:u,S=c>h?c>d?c:d:h>d?h:d;let M=i.next;for(;M!==e;){if(M.x>=m&&M.x<=g&&M.y>=_&&M.y<=S&&fo(o,c,a,h,u,d,M.x,M.y)&&Wt(M.prev,M,M.next)>=0)return!1;M=M.next}return!0}function YT(s,e,t,i){const o=s.prev,a=s,u=s.next;if(Wt(o,a,u)>=0)return!1;const c=o.x,h=a.x,d=u.x,m=o.y,_=a.y,g=u.y,S=c<h?c<d?c:d:h<d?h:d,M=m<_?m<g?m:g:_<g?_:g,E=c>h?c>d?c:d:h>d?h:d,y=m>_?m>g?m:g:_>g?_:g,x=ld(S,M,e,t,i),b=ld(E,y,e,t,i);let T=s.prevZ,P=s.nextZ;for(;T&&T.z>=x&&P&&P.z<=b;){if(T.x>=S&&T.x<=E&&T.y>=M&&T.y<=y&&T!==o&&T!==u&&fo(c,m,h,_,d,g,T.x,T.y)&&Wt(T.prev,T,T.next)>=0||(T=T.prevZ,P.x>=S&&P.x<=E&&P.y>=M&&P.y<=y&&P!==o&&P!==u&&fo(c,m,h,_,d,g,P.x,P.y)&&Wt(P.prev,P,P.next)>=0))return!1;P=P.nextZ}for(;T&&T.z>=x;){if(T.x>=S&&T.x<=E&&T.y>=M&&T.y<=y&&T!==o&&T!==u&&fo(c,m,h,_,d,g,T.x,T.y)&&Wt(T.prev,T,T.next)>=0)return!1;T=T.prevZ}for(;P&&P.z<=b;){if(P.x>=S&&P.x<=E&&P.y>=M&&P.y<=y&&P!==o&&P!==u&&fo(c,m,h,_,d,g,P.x,P.y)&&Wt(P.prev,P,P.next)>=0)return!1;P=P.nextZ}return!0}function KT(s,e,t){let i=s;do{const o=i.prev,a=i.next.next;!Fu(o,a)&&K_(o,i,i.next,a)&&Ea(o,a)&&Ea(a,o)&&(e.push(o.i/t|0),e.push(i.i/t|0),e.push(a.i/t|0),wa(i),wa(i.next),i=s=a),i=i.next}while(i!==s);return us(i)}function $T(s,e,t,i,o,a){let u=s;do{let c=u.next.next;for(;c!==u.prev;){if(u.i!==c.i&&sA(u,c)){let h=$_(u,c);u=us(u,u.next),h=us(h,h.next),Ma(u,e,t,i,o,a,0),Ma(h,e,t,i,o,a,0);return}c=c.next}u=u.next}while(u!==s)}function ZT(s,e,t,i){const o=[];let a,u,c,h,d;for(a=0,u=e.length;a<u;a++)c=e[a]*i,h=a<u-1?e[a+1]*i:s.length,d=Y_(s,c,h,i,!1),d===d.next&&(d.steiner=!0),o.push(rA(d));for(o.sort(JT),a=0;a<o.length;a++)t=QT(o[a],t);return t}function JT(s,e){return s.x-e.x}function QT(s,e){const t=eA(s,e);if(!t)return e;const i=$_(t,s);return us(i,i.next),us(t,t.next)}function eA(s,e){let t=e,i=-1/0,o;const a=s.x,u=s.y;do{if(u<=t.y&&u>=t.next.y&&t.next.y!==t.y){const g=t.x+(u-t.y)*(t.next.x-t.x)/(t.next.y-t.y);if(g<=a&&g>i&&(i=g,o=t.x<t.next.x?t:t.next,g===a))return o}t=t.next}while(t!==e);if(!o)return null;const c=o,h=o.x,d=o.y;let m=1/0,_;t=o;do a>=t.x&&t.x>=h&&a!==t.x&&fo(u<d?a:i,u,h,d,u<d?i:a,u,t.x,t.y)&&(_=Math.abs(u-t.y)/(a-t.x),Ea(t,s)&&(_<m||_===m&&(t.x>o.x||t.x===o.x&&tA(o,t)))&&(o=t,m=_)),t=t.next;while(t!==c);return o}function tA(s,e){return Wt(s.prev,s,e.prev)<0&&Wt(e.next,s,s.next)<0}function nA(s,e,t,i){let o=s;do o.z===0&&(o.z=ld(o.x,o.y,e,t,i)),o.prevZ=o.prev,o.nextZ=o.next,o=o.next;while(o!==s);o.prevZ.nextZ=null,o.prevZ=null,iA(o)}function iA(s){let e,t,i,o,a,u,c,h,d=1;do{for(t=s,s=null,a=null,u=0;t;){for(u++,i=t,c=0,e=0;e<d&&(c++,i=i.nextZ,!!i);e++);for(h=d;c>0||h>0&&i;)c!==0&&(h===0||!i||t.z<=i.z)?(o=t,t=t.nextZ,c--):(o=i,i=i.nextZ,h--),a?a.nextZ=o:s=o,o.prevZ=a,a=o;t=i}a.nextZ=null,d*=2}while(u>1);return s}function ld(s,e,t,i,o){return s=(s-t)*o|0,e=(e-i)*o|0,s=(s|s<<8)&16711935,s=(s|s<<4)&252645135,s=(s|s<<2)&858993459,s=(s|s<<1)&1431655765,e=(e|e<<8)&16711935,e=(e|e<<4)&252645135,e=(e|e<<2)&858993459,e=(e|e<<1)&1431655765,s|e<<1}function rA(s){let e=s,t=s;do(e.x<t.x||e.x===t.x&&e.y<t.y)&&(t=e),e=e.next;while(e!==s);return t}function fo(s,e,t,i,o,a,u,c){return(o-u)*(e-c)>=(s-u)*(a-c)&&(s-u)*(i-c)>=(t-u)*(e-c)&&(t-u)*(a-c)>=(o-u)*(i-c)}function sA(s,e){return s.next.i!==e.i&&s.prev.i!==e.i&&!oA(s,e)&&(Ea(s,e)&&Ea(e,s)&&aA(s,e)&&(Wt(s.prev,s,e.prev)||Wt(s,e.prev,e))||Fu(s,e)&&Wt(s.prev,s,s.next)>0&&Wt(e.prev,e,e.next)>0)}function Wt(s,e,t){return(e.y-s.y)*(t.x-e.x)-(e.x-s.x)*(t.y-e.y)}function Fu(s,e){return s.x===e.x&&s.y===e.y}function K_(s,e,t,i){const o=pu(Wt(s,e,t)),a=pu(Wt(s,e,i)),u=pu(Wt(t,i,s)),c=pu(Wt(t,i,e));return!!(o!==a&&u!==c||o===0&&du(s,t,e)||a===0&&du(s,i,e)||u===0&&du(t,s,i)||c===0&&du(t,e,i))}function du(s,e,t){return e.x<=Math.max(s.x,t.x)&&e.x>=Math.min(s.x,t.x)&&e.y<=Math.max(s.y,t.y)&&e.y>=Math.min(s.y,t.y)}function pu(s){return s>0?1:s<0?-1:0}function oA(s,e){let t=s;do{if(t.i!==s.i&&t.next.i!==s.i&&t.i!==e.i&&t.next.i!==e.i&&K_(t,t.next,s,e))return!0;t=t.next}while(t!==s);return!1}function Ea(s,e){return Wt(s.prev,s,s.next)<0?Wt(s,e,s.next)>=0&&Wt(s,s.prev,e)>=0:Wt(s,e,s.prev)<0||Wt(s,s.next,e)<0}function aA(s,e){let t=s,i=!1;const o=(s.x+e.x)/2,a=(s.y+e.y)/2;do t.y>a!=t.next.y>a&&t.next.y!==t.y&&o<(t.next.x-t.x)*(a-t.y)/(t.next.y-t.y)+t.x&&(i=!i),t=t.next;while(t!==s);return i}function $_(s,e){const t=new ud(s.i,s.x,s.y),i=new ud(e.i,e.x,e.y),o=s.next,a=e.prev;return s.next=e,e.prev=s,t.next=o,o.prev=t,i.next=t,t.prev=i,a.next=i,i.prev=a,i}function G0(s,e,t,i){const o=new ud(s,e,t);return i?(o.next=i.next,o.prev=i,i.next.prev=o,i.next=o):(o.prev=o,o.next=o),o}function wa(s){s.next.prev=s.prev,s.prev.next=s.next,s.prevZ&&(s.prevZ.nextZ=s.nextZ),s.nextZ&&(s.nextZ.prevZ=s.prevZ)}function ud(s,e,t){this.i=s,this.x=e,this.y=t,this.prev=null,this.next=null,this.z=0,this.prevZ=null,this.nextZ=null,this.steiner=!1}function lA(s,e,t,i){let o=0;for(let a=e,u=t-i;a<t;a+=i)o+=(s[u]-s[a])*(s[a+1]+s[u+1]),u=a;return o}class xa{static area(e){const t=e.length;let i=0;for(let o=t-1,a=0;a<t;o=a++)i+=e[o].x*e[a].y-e[a].x*e[o].y;return i*.5}static isClockWise(e){return xa.area(e)<0}static triangulateShape(e,t){const i=[],o=[],a=[];W0(e),X0(i,e);let u=e.length;t.forEach(W0);for(let h=0;h<t.length;h++)o.push(u),u+=t[h].length,X0(i,t[h]);const c=jT.triangulate(i,o);for(let h=0;h<c.length;h+=3)a.push(c.slice(h,h+3));return a}}function W0(s){const e=s.length;e>2&&s[e-1].equals(s[0])&&s.pop()}function X0(s,e){for(let t=0;t<e.length;t++)s.push(e[t].x),s.push(e[t].y)}class Cd extends un{constructor(e=new q_([new Pe(.5,.5),new Pe(-.5,.5),new Pe(-.5,-.5),new Pe(.5,-.5)]),t={}){super(),this.type="ExtrudeGeometry",this.parameters={shapes:e,options:t},e=Array.isArray(e)?e:[e];const i=this,o=[],a=[];for(let c=0,h=e.length;c<h;c++){const d=e[c];u(d)}this.setAttribute("position",new gn(o,3)),this.setAttribute("uv",new gn(a,2)),this.computeVertexNormals();function u(c){const h=[],d=t.curveSegments!==void 0?t.curveSegments:12,m=t.steps!==void 0?t.steps:1,_=t.depth!==void 0?t.depth:1;let g=t.bevelEnabled!==void 0?t.bevelEnabled:!0,S=t.bevelThickness!==void 0?t.bevelThickness:.2,M=t.bevelSize!==void 0?t.bevelSize:S-.1,E=t.bevelOffset!==void 0?t.bevelOffset:0,y=t.bevelSegments!==void 0?t.bevelSegments:3;const x=t.extrudePath,b=t.UVGenerator!==void 0?t.UVGenerator:uA;let T,P=!1,H,D,N,z;x&&(T=x.getSpacedPoints(m),P=!0,g=!1,H=x.computeFrenetFrames(m,!1),D=new W,N=new W,z=new W),g||(y=0,S=0,M=0,E=0);const C=c.extractPoints(d);let w=C.shape;const U=C.holes;if(!xa.isClockWise(w)){w=w.reverse();for(let I=0,Te=U.length;I<Te;I++){const Se=U[I];xa.isClockWise(Se)&&(U[I]=Se.reverse())}}const k=xa.triangulateShape(w,U),Y=w;for(let I=0,Te=U.length;I<Te;I++){const Se=U[I];w=w.concat(Se)}function ae(I,Te,Se){return Te||console.error("THREE.ExtrudeGeometry: vec does not exist"),I.clone().addScaledVector(Te,Se)}const te=w.length,ce=k.length;function G(I,Te,Se){let Oe,pe,et;const Be=I.x-Te.x,We=I.y-Te.y,F=Se.x-I.x,R=Se.y-I.y,oe=Be*Be+We*We,ve=Be*R-We*F;if(Math.abs(ve)>Number.EPSILON){const _e=Math.sqrt(oe),ye=Math.sqrt(F*F+R*R),Qe=Te.x-We/_e,Fe=Te.y+Be/_e,He=Se.x-R/ye,ut=Se.y+F/ye,Re=((He-Qe)*R-(ut-Fe)*F)/(Be*R-We*F);Oe=Qe+Be*Re-I.x,pe=Fe+We*Re-I.y;const ke=Oe*Oe+pe*pe;if(ke<=2)return new Pe(Oe,pe);et=Math.sqrt(ke/2)}else{let _e=!1;Be>Number.EPSILON?F>Number.EPSILON&&(_e=!0):Be<-Number.EPSILON?F<-Number.EPSILON&&(_e=!0):Math.sign(We)===Math.sign(R)&&(_e=!0),_e?(Oe=-We,pe=Be,et=Math.sqrt(oe)):(Oe=Be,pe=We,et=Math.sqrt(oe/2))}return new Pe(Oe/et,pe/et)}const he=[];for(let I=0,Te=Y.length,Se=Te-1,Oe=I+1;I<Te;I++,Se++,Oe++)Se===Te&&(Se=0),Oe===Te&&(Oe=0),he[I]=G(Y[I],Y[Se],Y[Oe]);const re=[];let O,ie=he.concat();for(let I=0,Te=U.length;I<Te;I++){const Se=U[I];O=[];for(let Oe=0,pe=Se.length,et=pe-1,Be=Oe+1;Oe<pe;Oe++,et++,Be++)et===pe&&(et=0),Be===pe&&(Be=0),O[Oe]=G(Se[Oe],Se[et],Se[Be]);re.push(O),ie=ie.concat(O)}for(let I=0;I<y;I++){const Te=I/y,Se=S*Math.cos(Te*Math.PI/2),Oe=M*Math.sin(Te*Math.PI/2)+E;for(let pe=0,et=Y.length;pe<et;pe++){const Be=ae(Y[pe],he[pe],Oe);de(Be.x,Be.y,-Se)}for(let pe=0,et=U.length;pe<et;pe++){const Be=U[pe];O=re[pe];for(let We=0,F=Be.length;We<F;We++){const R=ae(Be[We],O[We],Oe);de(R.x,R.y,-Se)}}}const Ae=M+E;for(let I=0;I<te;I++){const Te=g?ae(w[I],ie[I],Ae):w[I];P?(N.copy(H.normals[0]).multiplyScalar(Te.x),D.copy(H.binormals[0]).multiplyScalar(Te.y),z.copy(T[0]).add(N).add(D),de(z.x,z.y,z.z)):de(Te.x,Te.y,0)}for(let I=1;I<=m;I++)for(let Te=0;Te<te;Te++){const Se=g?ae(w[Te],ie[Te],Ae):w[Te];P?(N.copy(H.normals[I]).multiplyScalar(Se.x),D.copy(H.binormals[I]).multiplyScalar(Se.y),z.copy(T[I]).add(N).add(D),de(z.x,z.y,z.z)):de(Se.x,Se.y,_/m*I)}for(let I=y-1;I>=0;I--){const Te=I/y,Se=S*Math.cos(Te*Math.PI/2),Oe=M*Math.sin(Te*Math.PI/2)+E;for(let pe=0,et=Y.length;pe<et;pe++){const Be=ae(Y[pe],he[pe],Oe);de(Be.x,Be.y,_+Se)}for(let pe=0,et=U.length;pe<et;pe++){const Be=U[pe];O=re[pe];for(let We=0,F=Be.length;We<F;We++){const R=ae(Be[We],O[We],Oe);P?de(R.x,R.y+T[m-1].y,T[m-1].x+Se):de(R.x,R.y,_+Se)}}}Z(),ne();function Z(){const I=o.length/3;if(g){let Te=0,Se=te*Te;for(let Oe=0;Oe<ce;Oe++){const pe=k[Oe];ee(pe[2]+Se,pe[1]+Se,pe[0]+Se)}Te=m+y*2,Se=te*Te;for(let Oe=0;Oe<ce;Oe++){const pe=k[Oe];ee(pe[0]+Se,pe[1]+Se,pe[2]+Se)}}else{for(let Te=0;Te<ce;Te++){const Se=k[Te];ee(Se[2],Se[1],Se[0])}for(let Te=0;Te<ce;Te++){const Se=k[Te];ee(Se[0]+te*m,Se[1]+te*m,Se[2]+te*m)}}i.addGroup(I,o.length/3-I,0)}function ne(){const I=o.length/3;let Te=0;xe(Y,Te),Te+=Y.length;for(let Se=0,Oe=U.length;Se<Oe;Se++){const pe=U[Se];xe(pe,Te),Te+=pe.length}i.addGroup(I,o.length/3-I,1)}function xe(I,Te){let Se=I.length;for(;--Se>=0;){const Oe=Se;let pe=Se-1;pe<0&&(pe=I.length-1);for(let et=0,Be=m+y*2;et<Be;et++){const We=te*et,F=te*(et+1),R=Te+Oe+We,oe=Te+pe+We,ve=Te+pe+F,_e=Te+Oe+F;we(R,oe,ve,_e)}}}function de(I,Te,Se){h.push(I),h.push(Te),h.push(Se)}function ee(I,Te,Se){Ue(I),Ue(Te),Ue(Se);const Oe=o.length/3,pe=b.generateTopUV(i,o,Oe-3,Oe-2,Oe-1);tt(pe[0]),tt(pe[1]),tt(pe[2])}function we(I,Te,Se,Oe){Ue(I),Ue(Te),Ue(Oe),Ue(Te),Ue(Se),Ue(Oe);const pe=o.length/3,et=b.generateSideWallUV(i,o,pe-6,pe-3,pe-2,pe-1);tt(et[0]),tt(et[1]),tt(et[3]),tt(et[1]),tt(et[2]),tt(et[3])}function Ue(I){o.push(h[I*3+0]),o.push(h[I*3+1]),o.push(h[I*3+2])}function tt(I){a.push(I.x),a.push(I.y)}}}copy(e){return super.copy(e),this.parameters=Object.assign({},e.parameters),this}toJSON(){const e=super.toJSON(),t=this.parameters.shapes,i=this.parameters.options;return cA(t,i,e)}static fromJSON(e,t){const i=[];for(let a=0,u=e.shapes.length;a<u;a++){const c=t[e.shapes[a]];i.push(c)}const o=e.options.extrudePath;return o!==void 0&&(e.options.extrudePath=new ad[o.type]().fromJSON(o)),new Cd(i,e.options)}}const uA={generateTopUV:function(s,e,t,i,o){const a=e[t*3],u=e[t*3+1],c=e[i*3],h=e[i*3+1],d=e[o*3],m=e[o*3+1];return[new Pe(a,u),new Pe(c,h),new Pe(d,m)]},generateSideWallUV:function(s,e,t,i,o,a){const u=e[t*3],c=e[t*3+1],h=e[t*3+2],d=e[i*3],m=e[i*3+1],_=e[i*3+2],g=e[o*3],S=e[o*3+1],M=e[o*3+2],E=e[a*3],y=e[a*3+1],x=e[a*3+2];return Math.abs(c-m)<Math.abs(u-d)?[new Pe(u,1-h),new Pe(d,1-_),new Pe(g,1-M),new Pe(E,1-x)]:[new Pe(c,1-h),new Pe(m,1-_),new Pe(S,1-M),new Pe(y,1-x)]}};function cA(s,e,t){if(t.shapes=[],Array.isArray(s))for(let i=0,o=s.length;i<o;i++){const a=s[i];t.shapes.push(a.uuid)}else t.shapes.push(s.uuid);return t.options=Object.assign({},e),e.extrudePath!==void 0&&(t.options.extrudePath=e.extrudePath.toJSON()),t}class Cn extends ds{constructor(e){super(),this.isMeshStandardMaterial=!0,this.defines={STANDARD:""},this.type="MeshStandardMaterial",this.color=new yt(16777215),this.roughness=1,this.metalness=0,this.map=null,this.lightMap=null,this.lightMapIntensity=1,this.aoMap=null,this.aoMapIntensity=1,this.emissive=new yt(0),this.emissiveIntensity=1,this.emissiveMap=null,this.bumpMap=null,this.bumpScale=1,this.normalMap=null,this.normalMapType=S_,this.normalScale=new Pe(1,1),this.displacementMap=null,this.displacementScale=1,this.displacementBias=0,this.roughnessMap=null,this.metalnessMap=null,this.alphaMap=null,this.envMap=null,this.envMapRotation=new Ai,this.envMapIntensity=1,this.wireframe=!1,this.wireframeLinewidth=1,this.wireframeLinecap="round",this.wireframeLinejoin="round",this.flatShading=!1,this.fog=!0,this.setValues(e)}copy(e){return super.copy(e),this.defines={STANDARD:""},this.color.copy(e.color),this.roughness=e.roughness,this.metalness=e.metalness,this.map=e.map,this.lightMap=e.lightMap,this.lightMapIntensity=e.lightMapIntensity,this.aoMap=e.aoMap,this.aoMapIntensity=e.aoMapIntensity,this.emissive.copy(e.emissive),this.emissiveMap=e.emissiveMap,this.emissiveIntensity=e.emissiveIntensity,this.bumpMap=e.bumpMap,this.bumpScale=e.bumpScale,this.normalMap=e.normalMap,this.normalMapType=e.normalMapType,this.normalScale.copy(e.normalScale),this.displacementMap=e.displacementMap,this.displacementScale=e.displacementScale,this.displacementBias=e.displacementBias,this.roughnessMap=e.roughnessMap,this.metalnessMap=e.metalnessMap,this.alphaMap=e.alphaMap,this.envMap=e.envMap,this.envMapRotation.copy(e.envMapRotation),this.envMapIntensity=e.envMapIntensity,this.wireframe=e.wireframe,this.wireframeLinewidth=e.wireframeLinewidth,this.wireframeLinecap=e.wireframeLinecap,this.wireframeLinejoin=e.wireframeLinejoin,this.flatShading=e.flatShading,this.fog=e.fog,this}}class Z_ extends cn{constructor(e,t=1){super(),this.isLight=!0,this.type="Light",this.color=new yt(e),this.intensity=t}dispose(){}copy(e,t){return super.copy(e,t),this.color.copy(e.color),this.intensity=e.intensity,this}toJSON(e){const t=super.toJSON(e);return t.object.color=this.color.getHex(),t.object.intensity=this.intensity,this.groundColor!==void 0&&(t.object.groundColor=this.groundColor.getHex()),this.distance!==void 0&&(t.object.distance=this.distance),this.angle!==void 0&&(t.object.angle=this.angle),this.decay!==void 0&&(t.object.decay=this.decay),this.penumbra!==void 0&&(t.object.penumbra=this.penumbra),this.shadow!==void 0&&(t.object.shadow=this.shadow.toJSON()),this.target!==void 0&&(t.object.target=this.target.uuid),t}}const wh=new kt,j0=new W,q0=new W;class fA{constructor(e){this.camera=e,this.intensity=1,this.bias=0,this.normalBias=0,this.radius=1,this.blurSamples=8,this.mapSize=new Pe(512,512),this.map=null,this.mapPass=null,this.matrix=new kt,this.autoUpdate=!0,this.needsUpdate=!1,this._frustum=new Md,this._frameExtents=new Pe(1,1),this._viewportCount=1,this._viewports=[new Qt(0,0,1,1)]}getViewportCount(){return this._viewportCount}getFrustum(){return this._frustum}updateMatrices(e){const t=this.camera,i=this.matrix;j0.setFromMatrixPosition(e.matrixWorld),t.position.copy(j0),q0.setFromMatrixPosition(e.target.matrixWorld),t.lookAt(q0),t.updateMatrixWorld(),wh.multiplyMatrices(t.projectionMatrix,t.matrixWorldInverse),this._frustum.setFromProjectionMatrix(wh),i.set(.5,0,0,.5,0,.5,0,.5,0,0,.5,.5,0,0,0,1),i.multiply(wh)}getViewport(e){return this._viewports[e]}getFrameExtents(){return this._frameExtents}dispose(){this.map&&this.map.dispose(),this.mapPass&&this.mapPass.dispose()}copy(e){return this.camera=e.camera.clone(),this.intensity=e.intensity,this.bias=e.bias,this.radius=e.radius,this.mapSize.copy(e.mapSize),this}clone(){return new this.constructor().copy(this)}toJSON(){const e={};return this.intensity!==1&&(e.intensity=this.intensity),this.bias!==0&&(e.bias=this.bias),this.normalBias!==0&&(e.normalBias=this.normalBias),this.radius!==1&&(e.radius=this.radius),(this.mapSize.x!==512||this.mapSize.y!==512)&&(e.mapSize=this.mapSize.toArray()),e.camera=this.camera.toJSON(!1).object,delete e.camera.matrix,e}}class hA extends fA{constructor(){super(new U_(-5,5,5,-5,.5,500)),this.isDirectionalLightShadow=!0}}class Y0 extends Z_{constructor(e,t){super(e,t),this.isDirectionalLight=!0,this.type="DirectionalLight",this.position.copy(cn.DEFAULT_UP),this.updateMatrix(),this.target=new cn,this.shadow=new hA}dispose(){this.shadow.dispose()}copy(e){return super.copy(e),this.target=e.target.clone(),this.shadow=e.shadow.clone(),this}}class dA extends Z_{constructor(e,t){super(e,t),this.isAmbientLight=!0,this.type="AmbientLight"}}class K0{constructor(e=1,t=0,i=0){return this.radius=e,this.phi=t,this.theta=i,this}set(e,t,i){return this.radius=e,this.phi=t,this.theta=i,this}copy(e){return this.radius=e.radius,this.phi=e.phi,this.theta=e.theta,this}makeSafe(){return this.phi=Math.max(1e-6,Math.min(Math.PI-1e-6,this.phi)),this}setFromVector3(e){return this.setFromCartesianCoords(e.x,e.y,e.z)}setFromCartesianCoords(e,t,i){return this.radius=Math.sqrt(e*e+t*t+i*i),this.radius===0?(this.theta=0,this.phi=0):(this.theta=Math.atan2(e,i),this.phi=Math.acos(pn(t/this.radius,-1,1))),this}clone(){return new this.constructor().copy(this)}}class pA extends V_{constructor(e=10,t=10,i=4473924,o=8947848){i=new yt(i),o=new yt(o);const a=t/2,u=e/t,c=e/2,h=[],d=[];for(let g=0,S=0,M=-c;g<=t;g++,M+=u){h.push(-c,0,M,c,0,M),h.push(M,0,-c,M,0,c);const E=g===a?i:o;E.toArray(d,S),S+=3,E.toArray(d,S),S+=3,E.toArray(d,S),S+=3,E.toArray(d,S),S+=3}const m=new un;m.setAttribute("position",new gn(h,3)),m.setAttribute("color",new gn(d,3));const _=new Pr({vertexColors:!0,toneMapped:!1});super(m,_),this.type="GridHelper"}dispose(){this.geometry.dispose(),this.material.dispose()}}class mA extends hs{constructor(e,t){super(),this.object=e,this.domElement=t,this.enabled=!0,this.state=-1,this.keys={},this.mouseButtons={LEFT:null,MIDDLE:null,RIGHT:null},this.touches={ONE:null,TWO:null}}connect(){}disconnect(){}dispose(){}update(){}}typeof __THREE_DEVTOOLS__<"u"&&__THREE_DEVTOOLS__.dispatchEvent(new CustomEvent("register",{detail:{revision:dd}}));typeof window<"u"&&(window.__THREE__?console.warn("WARNING: Multiple instances of Three.js being imported."):window.__THREE__=dd);const $0={type:"change"},Rd={type:"start"},J_={type:"end"},mu=new Sd,Z0=new Cr,gA=Math.cos(70*vS.DEG2RAD),Jt=new W,Fn=2*Math.PI,Pt={NONE:-1,ROTATE:0,DOLLY:1,PAN:2,TOUCH_ROTATE:3,TOUCH_PAN:4,TOUCH_DOLLY_PAN:5,TOUCH_DOLLY_ROTATE:6},Th=1e-6;class _A extends mA{constructor(e,t=null){super(e,t),this.state=Pt.NONE,this.enabled=!0,this.target=new W,this.cursor=new W,this.minDistance=0,this.maxDistance=1/0,this.minZoom=0,this.maxZoom=1/0,this.minTargetRadius=0,this.maxTargetRadius=1/0,this.minPolarAngle=0,this.maxPolarAngle=Math.PI,this.minAzimuthAngle=-1/0,this.maxAzimuthAngle=1/0,this.enableDamping=!1,this.dampingFactor=.05,this.enableZoom=!0,this.zoomSpeed=1,this.enableRotate=!0,this.rotateSpeed=1,this.enablePan=!0,this.panSpeed=1,this.screenSpacePanning=!0,this.keyPanSpeed=7,this.zoomToCursor=!1,this.autoRotate=!1,this.autoRotateSpeed=2,this.keys={LEFT:"ArrowLeft",UP:"ArrowUp",RIGHT:"ArrowRight",BOTTOM:"ArrowDown"},this.mouseButtons={LEFT:ho.ROTATE,MIDDLE:ho.DOLLY,RIGHT:ho.PAN},this.touches={ONE:uo.ROTATE,TWO:uo.DOLLY_PAN},this.target0=this.target.clone(),this.position0=this.object.position.clone(),this.zoom0=this.object.zoom,this._domElementKeyEvents=null,this._lastPosition=new W,this._lastQuaternion=new ls,this._lastTargetPosition=new W,this._quat=new ls().setFromUnitVectors(e.up,new W(0,1,0)),this._quatInverse=this._quat.clone().invert(),this._spherical=new K0,this._sphericalDelta=new K0,this._scale=1,this._panOffset=new W,this._rotateStart=new Pe,this._rotateEnd=new Pe,this._rotateDelta=new Pe,this._panStart=new Pe,this._panEnd=new Pe,this._panDelta=new Pe,this._dollyStart=new Pe,this._dollyEnd=new Pe,this._dollyDelta=new Pe,this._dollyDirection=new W,this._mouse=new Pe,this._performCursorZoom=!1,this._pointers=[],this._pointerPositions={},this._controlActive=!1,this._onPointerMove=yA.bind(this),this._onPointerDown=vA.bind(this),this._onPointerUp=xA.bind(this),this._onContextMenu=CA.bind(this),this._onMouseWheel=EA.bind(this),this._onKeyDown=wA.bind(this),this._onTouchStart=TA.bind(this),this._onTouchMove=AA.bind(this),this._onMouseDown=SA.bind(this),this._onMouseMove=MA.bind(this),this._interceptControlDown=RA.bind(this),this._interceptControlUp=PA.bind(this),this.domElement!==null&&this.connect(),this.update()}connect(){this.domElement.addEventListener("pointerdown",this._onPointerDown),this.domElement.addEventListener("pointercancel",this._onPointerUp),this.domElement.addEventListener("contextmenu",this._onContextMenu),this.domElement.addEventListener("wheel",this._onMouseWheel,{passive:!1}),this.domElement.getRootNode().addEventListener("keydown",this._interceptControlDown,{passive:!0,capture:!0}),this.domElement.style.touchAction="none"}disconnect(){this.domElement.removeEventListener("pointerdown",this._onPointerDown),this.domElement.removeEventListener("pointermove",this._onPointerMove),this.domElement.removeEventListener("pointerup",this._onPointerUp),this.domElement.removeEventListener("pointercancel",this._onPointerUp),this.domElement.removeEventListener("wheel",this._onMouseWheel),this.domElement.removeEventListener("contextmenu",this._onContextMenu),this.stopListenToKeyEvents(),this.domElement.getRootNode().removeEventListener("keydown",this._interceptControlDown,{capture:!0}),this.domElement.style.touchAction="auto"}dispose(){this.disconnect()}getPolarAngle(){return this._spherical.phi}getAzimuthalAngle(){return this._spherical.theta}getDistance(){return this.object.position.distanceTo(this.target)}listenToKeyEvents(e){e.addEventListener("keydown",this._onKeyDown),this._domElementKeyEvents=e}stopListenToKeyEvents(){this._domElementKeyEvents!==null&&(this._domElementKeyEvents.removeEventListener("keydown",this._onKeyDown),this._domElementKeyEvents=null)}saveState(){this.target0.copy(this.target),this.position0.copy(this.object.position),this.zoom0=this.object.zoom}reset(){this.target.copy(this.target0),this.object.position.copy(this.position0),this.object.zoom=this.zoom0,this.object.updateProjectionMatrix(),this.dispatchEvent($0),this.update(),this.state=Pt.NONE}update(e=null){const t=this.object.position;Jt.copy(t).sub(this.target),Jt.applyQuaternion(this._quat),this._spherical.setFromVector3(Jt),this.autoRotate&&this.state===Pt.NONE&&this._rotateLeft(this._getAutoRotationAngle(e)),this.enableDamping?(this._spherical.theta+=this._sphericalDelta.theta*this.dampingFactor,this._spherical.phi+=this._sphericalDelta.phi*this.dampingFactor):(this._spherical.theta+=this._sphericalDelta.theta,this._spherical.phi+=this._sphericalDelta.phi);let i=this.minAzimuthAngle,o=this.maxAzimuthAngle;isFinite(i)&&isFinite(o)&&(i<-Math.PI?i+=Fn:i>Math.PI&&(i-=Fn),o<-Math.PI?o+=Fn:o>Math.PI&&(o-=Fn),i<=o?this._spherical.theta=Math.max(i,Math.min(o,this._spherical.theta)):this._spherical.theta=this._spherical.theta>(i+o)/2?Math.max(i,this._spherical.theta):Math.min(o,this._spherical.theta)),this._spherical.phi=Math.max(this.minPolarAngle,Math.min(this.maxPolarAngle,this._spherical.phi)),this._spherical.makeSafe(),this.enableDamping===!0?this.target.addScaledVector(this._panOffset,this.dampingFactor):this.target.add(this._panOffset),this.target.sub(this.cursor),this.target.clampLength(this.minTargetRadius,this.maxTargetRadius),this.target.add(this.cursor);let a=!1;if(this.zoomToCursor&&this._performCursorZoom||this.object.isOrthographicCamera)this._spherical.radius=this._clampDistance(this._spherical.radius);else{const u=this._spherical.radius;this._spherical.radius=this._clampDistance(this._spherical.radius*this._scale),a=u!=this._spherical.radius}if(Jt.setFromSpherical(this._spherical),Jt.applyQuaternion(this._quatInverse),t.copy(this.target).add(Jt),this.object.lookAt(this.target),this.enableDamping===!0?(this._sphericalDelta.theta*=1-this.dampingFactor,this._sphericalDelta.phi*=1-this.dampingFactor,this._panOffset.multiplyScalar(1-this.dampingFactor)):(this._sphericalDelta.set(0,0,0),this._panOffset.set(0,0,0)),this.zoomToCursor&&this._performCursorZoom){let u=null;if(this.object.isPerspectiveCamera){const c=Jt.length();u=this._clampDistance(c*this._scale);const h=c-u;this.object.position.addScaledVector(this._dollyDirection,h),this.object.updateMatrixWorld(),a=!!h}else if(this.object.isOrthographicCamera){const c=new W(this._mouse.x,this._mouse.y,0);c.unproject(this.object);const h=this.object.zoom;this.object.zoom=Math.max(this.minZoom,Math.min(this.maxZoom,this.object.zoom/this._scale)),this.object.updateProjectionMatrix(),a=h!==this.object.zoom;const d=new W(this._mouse.x,this._mouse.y,0);d.unproject(this.object),this.object.position.sub(d).add(c),this.object.updateMatrixWorld(),u=Jt.length()}else console.warn("WARNING: OrbitControls.js encountered an unknown camera type - zoom to cursor disabled."),this.zoomToCursor=!1;u!==null&&(this.screenSpacePanning?this.target.set(0,0,-1).transformDirection(this.object.matrix).multiplyScalar(u).add(this.object.position):(mu.origin.copy(this.object.position),mu.direction.set(0,0,-1).transformDirection(this.object.matrix),Math.abs(this.object.up.dot(mu.direction))<gA?this.object.lookAt(this.target):(Z0.setFromNormalAndCoplanarPoint(this.object.up,this.target),mu.intersectPlane(Z0,this.target))))}else if(this.object.isOrthographicCamera){const u=this.object.zoom;this.object.zoom=Math.max(this.minZoom,Math.min(this.maxZoom,this.object.zoom/this._scale)),u!==this.object.zoom&&(this.object.updateProjectionMatrix(),a=!0)}return this._scale=1,this._performCursorZoom=!1,a||this._lastPosition.distanceToSquared(this.object.position)>Th||8*(1-this._lastQuaternion.dot(this.object.quaternion))>Th||this._lastTargetPosition.distanceToSquared(this.target)>Th?(this.dispatchEvent($0),this._lastPosition.copy(this.object.position),this._lastQuaternion.copy(this.object.quaternion),this._lastTargetPosition.copy(this.target),!0):!1}_getAutoRotationAngle(e){return e!==null?Fn/60*this.autoRotateSpeed*e:Fn/60/60*this.autoRotateSpeed}_getZoomScale(e){const t=Math.abs(e*.01);return Math.pow(.95,this.zoomSpeed*t)}_rotateLeft(e){this._sphericalDelta.theta-=e}_rotateUp(e){this._sphericalDelta.phi-=e}_panLeft(e,t){Jt.setFromMatrixColumn(t,0),Jt.multiplyScalar(-e),this._panOffset.add(Jt)}_panUp(e,t){this.screenSpacePanning===!0?Jt.setFromMatrixColumn(t,1):(Jt.setFromMatrixColumn(t,0),Jt.crossVectors(this.object.up,Jt)),Jt.multiplyScalar(e),this._panOffset.add(Jt)}_pan(e,t){const i=this.domElement;if(this.object.isPerspectiveCamera){const o=this.object.position;Jt.copy(o).sub(this.target);let a=Jt.length();a*=Math.tan(this.object.fov/2*Math.PI/180),this._panLeft(2*e*a/i.clientHeight,this.object.matrix),this._panUp(2*t*a/i.clientHeight,this.object.matrix)}else this.object.isOrthographicCamera?(this._panLeft(e*(this.object.right-this.object.left)/this.object.zoom/i.clientWidth,this.object.matrix),this._panUp(t*(this.object.top-this.object.bottom)/this.object.zoom/i.clientHeight,this.object.matrix)):(console.warn("WARNING: OrbitControls.js encountered an unknown camera type - pan disabled."),this.enablePan=!1)}_dollyOut(e){this.object.isPerspectiveCamera||this.object.isOrthographicCamera?this._scale/=e:(console.warn("WARNING: OrbitControls.js encountered an unknown camera type - dolly/zoom disabled."),this.enableZoom=!1)}_dollyIn(e){this.object.isPerspectiveCamera||this.object.isOrthographicCamera?this._scale*=e:(console.warn("WARNING: OrbitControls.js encountered an unknown camera type - dolly/zoom disabled."),this.enableZoom=!1)}_updateZoomParameters(e,t){if(!this.zoomToCursor)return;this._performCursorZoom=!0;const i=this.domElement.getBoundingClientRect(),o=e-i.left,a=t-i.top,u=i.width,c=i.height;this._mouse.x=o/u*2-1,this._mouse.y=-(a/c)*2+1,this._dollyDirection.set(this._mouse.x,this._mouse.y,1).unproject(this.object).sub(this.object.position).normalize()}_clampDistance(e){return Math.max(this.minDistance,Math.min(this.maxDistance,e))}_handleMouseDownRotate(e){this._rotateStart.set(e.clientX,e.clientY)}_handleMouseDownDolly(e){this._updateZoomParameters(e.clientX,e.clientX),this._dollyStart.set(e.clientX,e.clientY)}_handleMouseDownPan(e){this._panStart.set(e.clientX,e.clientY)}_handleMouseMoveRotate(e){this._rotateEnd.set(e.clientX,e.clientY),this._rotateDelta.subVectors(this._rotateEnd,this._rotateStart).multiplyScalar(this.rotateSpeed);const t=this.domElement;this._rotateLeft(Fn*this._rotateDelta.x/t.clientHeight),this._rotateUp(Fn*this._rotateDelta.y/t.clientHeight),this._rotateStart.copy(this._rotateEnd),this.update()}_handleMouseMoveDolly(e){this._dollyEnd.set(e.clientX,e.clientY),this._dollyDelta.subVectors(this._dollyEnd,this._dollyStart),this._dollyDelta.y>0?this._dollyOut(this._getZoomScale(this._dollyDelta.y)):this._dollyDelta.y<0&&this._dollyIn(this._getZoomScale(this._dollyDelta.y)),this._dollyStart.copy(this._dollyEnd),this.update()}_handleMouseMovePan(e){this._panEnd.set(e.clientX,e.clientY),this._panDelta.subVectors(this._panEnd,this._panStart).multiplyScalar(this.panSpeed),this._pan(this._panDelta.x,this._panDelta.y),this._panStart.copy(this._panEnd),this.update()}_handleMouseWheel(e){this._updateZoomParameters(e.clientX,e.clientY),e.deltaY<0?this._dollyIn(this._getZoomScale(e.deltaY)):e.deltaY>0&&this._dollyOut(this._getZoomScale(e.deltaY)),this.update()}_handleKeyDown(e){let t=!1;switch(e.code){case this.keys.UP:e.ctrlKey||e.metaKey||e.shiftKey?this._rotateUp(Fn*this.rotateSpeed/this.domElement.clientHeight):this._pan(0,this.keyPanSpeed),t=!0;break;case this.keys.BOTTOM:e.ctrlKey||e.metaKey||e.shiftKey?this._rotateUp(-Fn*this.rotateSpeed/this.domElement.clientHeight):this._pan(0,-this.keyPanSpeed),t=!0;break;case this.keys.LEFT:e.ctrlKey||e.metaKey||e.shiftKey?this._rotateLeft(Fn*this.rotateSpeed/this.domElement.clientHeight):this._pan(this.keyPanSpeed,0),t=!0;break;case this.keys.RIGHT:e.ctrlKey||e.metaKey||e.shiftKey?this._rotateLeft(-Fn*this.rotateSpeed/this.domElement.clientHeight):this._pan(-this.keyPanSpeed,0),t=!0;break}t&&(e.preventDefault(),this.update())}_handleTouchStartRotate(e){if(this._pointers.length===1)this._rotateStart.set(e.pageX,e.pageY);else{const t=this._getSecondPointerPosition(e),i=.5*(e.pageX+t.x),o=.5*(e.pageY+t.y);this._rotateStart.set(i,o)}}_handleTouchStartPan(e){if(this._pointers.length===1)this._panStart.set(e.pageX,e.pageY);else{const t=this._getSecondPointerPosition(e),i=.5*(e.pageX+t.x),o=.5*(e.pageY+t.y);this._panStart.set(i,o)}}_handleTouchStartDolly(e){const t=this._getSecondPointerPosition(e),i=e.pageX-t.x,o=e.pageY-t.y,a=Math.sqrt(i*i+o*o);this._dollyStart.set(0,a)}_handleTouchStartDollyPan(e){this.enableZoom&&this._handleTouchStartDolly(e),this.enablePan&&this._handleTouchStartPan(e)}_handleTouchStartDollyRotate(e){this.enableZoom&&this._handleTouchStartDolly(e),this.enableRotate&&this._handleTouchStartRotate(e)}_handleTouchMoveRotate(e){if(this._pointers.length==1)this._rotateEnd.set(e.pageX,e.pageY);else{const i=this._getSecondPointerPosition(e),o=.5*(e.pageX+i.x),a=.5*(e.pageY+i.y);this._rotateEnd.set(o,a)}this._rotateDelta.subVectors(this._rotateEnd,this._rotateStart).multiplyScalar(this.rotateSpeed);const t=this.domElement;this._rotateLeft(Fn*this._rotateDelta.x/t.clientHeight),this._rotateUp(Fn*this._rotateDelta.y/t.clientHeight),this._rotateStart.copy(this._rotateEnd)}_handleTouchMovePan(e){if(this._pointers.length===1)this._panEnd.set(e.pageX,e.pageY);else{const t=this._getSecondPointerPosition(e),i=.5*(e.pageX+t.x),o=.5*(e.pageY+t.y);this._panEnd.set(i,o)}this._panDelta.subVectors(this._panEnd,this._panStart).multiplyScalar(this.panSpeed),this._pan(this._panDelta.x,this._panDelta.y),this._panStart.copy(this._panEnd)}_handleTouchMoveDolly(e){const t=this._getSecondPointerPosition(e),i=e.pageX-t.x,o=e.pageY-t.y,a=Math.sqrt(i*i+o*o);this._dollyEnd.set(0,a),this._dollyDelta.set(0,Math.pow(this._dollyEnd.y/this._dollyStart.y,this.zoomSpeed)),this._dollyOut(this._dollyDelta.y),this._dollyStart.copy(this._dollyEnd);const u=(e.pageX+t.x)*.5,c=(e.pageY+t.y)*.5;this._updateZoomParameters(u,c)}_handleTouchMoveDollyPan(e){this.enableZoom&&this._handleTouchMoveDolly(e),this.enablePan&&this._handleTouchMovePan(e)}_handleTouchMoveDollyRotate(e){this.enableZoom&&this._handleTouchMoveDolly(e),this.enableRotate&&this._handleTouchMoveRotate(e)}_addPointer(e){this._pointers.push(e.pointerId)}_removePointer(e){delete this._pointerPositions[e.pointerId];for(let t=0;t<this._pointers.length;t++)if(this._pointers[t]==e.pointerId){this._pointers.splice(t,1);return}}_isTrackingPointer(e){for(let t=0;t<this._pointers.length;t++)if(this._pointers[t]==e.pointerId)return!0;return!1}_trackPointer(e){let t=this._pointerPositions[e.pointerId];t===void 0&&(t=new Pe,this._pointerPositions[e.pointerId]=t),t.set(e.pageX,e.pageY)}_getSecondPointerPosition(e){const t=e.pointerId===this._pointers[0]?this._pointers[1]:this._pointers[0];return this._pointerPositions[t]}_customWheelEvent(e){const t=e.deltaMode,i={clientX:e.clientX,clientY:e.clientY,deltaY:e.deltaY};switch(t){case 1:i.deltaY*=16;break;case 2:i.deltaY*=100;break}return e.ctrlKey&&!this._controlActive&&(i.deltaY*=10),i}}function vA(s){this.enabled!==!1&&(this._pointers.length===0&&(this.domElement.setPointerCapture(s.pointerId),this.domElement.addEventListener("pointermove",this._onPointerMove),this.domElement.addEventListener("pointerup",this._onPointerUp)),!this._isTrackingPointer(s)&&(this._addPointer(s),s.pointerType==="touch"?this._onTouchStart(s):this._onMouseDown(s)))}function yA(s){this.enabled!==!1&&(s.pointerType==="touch"?this._onTouchMove(s):this._onMouseMove(s))}function xA(s){switch(this._removePointer(s),this._pointers.length){case 0:this.domElement.releasePointerCapture(s.pointerId),this.domElement.removeEventListener("pointermove",this._onPointerMove),this.domElement.removeEventListener("pointerup",this._onPointerUp),this.dispatchEvent(J_),this.state=Pt.NONE;break;case 1:const e=this._pointers[0],t=this._pointerPositions[e];this._onTouchStart({pointerId:e,pageX:t.x,pageY:t.y});break}}function SA(s){let e;switch(s.button){case 0:e=this.mouseButtons.LEFT;break;case 1:e=this.mouseButtons.MIDDLE;break;case 2:e=this.mouseButtons.RIGHT;break;default:e=-1}switch(e){case ho.DOLLY:if(this.enableZoom===!1)return;this._handleMouseDownDolly(s),this.state=Pt.DOLLY;break;case ho.ROTATE:if(s.ctrlKey||s.metaKey||s.shiftKey){if(this.enablePan===!1)return;this._handleMouseDownPan(s),this.state=Pt.PAN}else{if(this.enableRotate===!1)return;this._handleMouseDownRotate(s),this.state=Pt.ROTATE}break;case ho.PAN:if(s.ctrlKey||s.metaKey||s.shiftKey){if(this.enableRotate===!1)return;this._handleMouseDownRotate(s),this.state=Pt.ROTATE}else{if(this.enablePan===!1)return;this._handleMouseDownPan(s),this.state=Pt.PAN}break;default:this.state=Pt.NONE}this.state!==Pt.NONE&&this.dispatchEvent(Rd)}function MA(s){switch(this.state){case Pt.ROTATE:if(this.enableRotate===!1)return;this._handleMouseMoveRotate(s);break;case Pt.DOLLY:if(this.enableZoom===!1)return;this._handleMouseMoveDolly(s);break;case Pt.PAN:if(this.enablePan===!1)return;this._handleMouseMovePan(s);break}}function EA(s){this.enabled===!1||this.enableZoom===!1||this.state!==Pt.NONE||(s.preventDefault(),this.dispatchEvent(Rd),this._handleMouseWheel(this._customWheelEvent(s)),this.dispatchEvent(J_))}function wA(s){this.enabled===!1||this.enablePan===!1||this._handleKeyDown(s)}function TA(s){switch(this._trackPointer(s),this._pointers.length){case 1:switch(this.touches.ONE){case uo.ROTATE:if(this.enableRotate===!1)return;this._handleTouchStartRotate(s),this.state=Pt.TOUCH_ROTATE;break;case uo.PAN:if(this.enablePan===!1)return;this._handleTouchStartPan(s),this.state=Pt.TOUCH_PAN;break;default:this.state=Pt.NONE}break;case 2:switch(this.touches.TWO){case uo.DOLLY_PAN:if(this.enableZoom===!1&&this.enablePan===!1)return;this._handleTouchStartDollyPan(s),this.state=Pt.TOUCH_DOLLY_PAN;break;case uo.DOLLY_ROTATE:if(this.enableZoom===!1&&this.enableRotate===!1)return;this._handleTouchStartDollyRotate(s),this.state=Pt.TOUCH_DOLLY_ROTATE;break;default:this.state=Pt.NONE}break;default:this.state=Pt.NONE}this.state!==Pt.NONE&&this.dispatchEvent(Rd)}function AA(s){switch(this._trackPointer(s),this.state){case Pt.TOUCH_ROTATE:if(this.enableRotate===!1)return;this._handleTouchMoveRotate(s),this.update();break;case Pt.TOUCH_PAN:if(this.enablePan===!1)return;this._handleTouchMovePan(s),this.update();break;case Pt.TOUCH_DOLLY_PAN:if(this.enableZoom===!1&&this.enablePan===!1)return;this._handleTouchMoveDollyPan(s),this.update();break;case Pt.TOUCH_DOLLY_ROTATE:if(this.enableZoom===!1&&this.enableRotate===!1)return;this._handleTouchMoveDollyRotate(s),this.update();break;default:this.state=Pt.NONE}}function CA(s){this.enabled!==!1&&s.preventDefault()}function RA(s){s.key==="Control"&&(this._controlActive=!0,this.domElement.getRootNode().addEventListener("keyup",this._interceptControlUp,{passive:!0,capture:!0}))}function PA(s){s.key==="Control"&&(this._controlActive=!1,this.domElement.getRootNode().removeEventListener("keyup",this._interceptControlUp,{passive:!0,capture:!0}))}function bA(s,e,t){const i=new qi,o=new On(s,t,e,1,1,1),a=new Cn({color:15259063,roughness:.4,metalness:.05}),u=new zt(o,a);u.castShadow=!0,u.receiveShadow=!0,i.add(u);const c=new XT(o),h=new Pr({color:4473924});return i.add(new V_(c,h)),i}function LA(s,e,t,i,o){var M;const a=new qi,u=s.length,c=((M=s[0])==null?void 0:M.length)||0,h=c*t,d=u*t,[m,_]=e,g=new On(t*.85,i,t*.85),S=new Cn({color:1710618,roughness:.6,metalness:.2});for(let E=0;E<u;E++)for(let y=0;y<c;y++){if(!s[E][y])continue;const x=m-h/2+y*t+t/2,b=_-d/2+E*t+t/2,T=new zt(g,S.clone());T.position.set(x,o+i/2,b),T.castShadow=!0,a.add(T)}return a}function J0(s,e,t,i=!1){const o=new qi,a=document.createElement("canvas");a.width=512,a.height=128;const u=a.getContext("2d");u.fillStyle="#ffffff",u.fillRect(0,0,a.width,a.height),u.fillStyle="#000000",u.font="bold 48px Arial",u.textAlign="center",u.textBaseline="middle",u.fillText(s,a.width/2,a.height/2);const c=new lo(a);c.minFilter=mn;const h=new Ra(1,.25),d=new Cn({map:c,transparent:!0,roughness:.8,side:wi}),m=new zt(h,d);return m.position.set(e[0],e[1],i?-e[2]:e[2]),i&&(m.rotation.y=Math.PI),o.add(m),o}function DA(s,e){const t=new qi;if(e==="horizontal"){const{plateVerts:i,hourLines:o,gnomonAngle:a,gnomonBaseLen:u,gnomonHeight:c,gnomonThickness:h}=s,d=new On(i[1][0]-i[0][0],2,i[2][2]-i[0][2]),m=new Cn({color:13944233,roughness:.5,metalness:.1}),_=new zt(d,m);_.position.y=1,_.castShadow=!0,_.receiveShadow=!0,t.add(_);for(const x of o){const b=(i[1][0]-i[0][0])*.4,T=Math.sin(x.angle)*b,P=Math.cos(x.angle)*b,H=[new W(0,2.1,0),new W(T,2.1,P)],D=new un().setFromPoints(H),N=new ao(D,new Pr({color:3355443,linewidth:1}));t.add(N)}const g=new q_;g.moveTo(-h/2,-.5),g.lineTo(h/2,-.5),g.lineTo(h/2,c),g.lineTo(-h/2,c);const S={steps:1,depth:u,bevelEnabled:!1},M=new Cd(g,S);M.rotateZ(a);const E=new Cn({color:8947848,roughness:.3,metalness:.7}),y=new zt(M,E);y.position.y=2,y.rotation.x=-Math.PI/2,y.castShadow=!0,t.add(y);for(let x=6;x<=18;x++){const b=o.find(X=>X.hour===x);if(!b&&x!==12)continue;const T=x===12?0:b.angle,P=(i[1][0]-i[0][0])*.35,H=Math.sin(T)*P,D=Math.cos(T)*P,N=document.createElement("canvas");N.width=64,N.height=64;const z=N.getContext("2d");z.fillStyle="#000",z.font="bold 32px Arial",z.textAlign="center",z.textBaseline="middle",z.fillText(String(x),32,32);const C=new lo(N);C.minFilter=mn;const w=new oo({map:C}),U=new pa(w);U.position.set(H,2.5,D),U.scale.set(8,8,1),t.add(U)}}if(e==="equatorial"){const{plateVerts:i,hourLines:o,tiltAngle:a,gnomonLength:u}=s,c=(i[1][0]-i[0][0])/2,h=new vo(c,c,3,32),d=new Cn({color:13944233,roughness:.5,metalness:.1}),m=new zt(h,d);m.rotation.z=a,m.castShadow=!0,m.receiveShadow=!0,t.add(m);for(const M of o){const E=c*.75,y=M.angle,x=new W(0,0,0),b=new W(Math.cos(y)*E,0,Math.sin(y)*E),T=new un().setFromPoints([x,b]),P=new ao(T,new Pr({color:3355443}));P.rotation.z=a,P.position.y=1.5,t.add(P)}const _=new vo(1.5,1.5,u,8),g=new Cn({color:8947848,roughness:.3,metalness:.7}),S=new zt(_,g);S.rotation.z=a-Math.PI/2,S.position.y=u/2,t.add(S);for(const M of o){const E=c*.85,y=document.createElement("canvas");y.width=64,y.height=64;const x=y.getContext("2d");x.fillStyle="#000",x.font="bold 32px Arial",x.textAlign="center",x.textBaseline="middle",x.fillText(String(M.hour),32,32);const b=new lo(y);b.minFilter=mn;const T=new pa(new oo({map:b})),P=M.angle;T.position.set(Math.cos(P)*E,2,Math.sin(P)*E),T.scale.set(6,6,1),T.rotation.z=a,t.add(T)}}if(e==="analemmatic"){const{markers:i,scaleX:o,scaleY:a,dateScale:u}=s,c=new On(o*2.5,1,o*2.5),h=new Cn({color:13944233,roughness:.5,metalness:.1}),d=new zt(c,h);d.position.y=.5,d.castShadow=!0,d.receiveShadow=!0,t.add(d);const m=[];for(let E=0;E<=64;E++){const y=E/64*Math.PI*2;m.push(new W(Math.sin(y)*o,1.1,Math.cos(y)*a))}const _=new un().setFromPoints(m),g=new ao(_,new Pr({color:3355443}));t.add(g);const S=[new W(0,1.1,-o),new W(0,1.1,o)],M=new un().setFromPoints(S);t.add(new ao(M,new Pr({color:11158596})));for(const E of i){const y=new vo(1.5,1.5,10,8),x=new zt(y,new Cn({color:6710886,roughness:.3,metalness:.5}));x.position.set(E.x,6,E.y),x.castShadow=!0,t.add(x);const b=document.createElement("canvas");b.width=64,b.height=64;const T=b.getContext("2d");T.fillStyle="#000",T.font="bold 32px Arial",T.textAlign="center",T.textBaseline="middle",T.fillText(String(E.hour),32,32);const P=new lo(b);P.minFilter=mn;const H=new pa(new oo({map:P}));H.position.set(E.x,12,E.y),H.scale.set(6,6,1),t.add(H)}for(const E of u){const y=new On(2,1,3),x=new zt(y,new Cn({color:11158596}));x.position.set(0,1.1,E.distance),t.add(x)}}if(e==="vertical"){const{plateVerts:i,hourLines:o,gnomonHeight:a}=s,u=i[1][0]-i[0][0],c=i[2][1]-i[0][1],h=new On(u,c,2),d=new Cn({color:13944233,roughness:.5,metalness:.1}),m=new zt(h,d);m.castShadow=!0,m.receiveShadow=!0,t.add(m);const _=new Ad(3,a,8),g=new zt(_,new Cn({color:8947848,roughness:.3,metalness:.7}));g.position.set(0,0,a/2+1),g.rotation.x=Math.PI/2,g.castShadow=!0,t.add(g);for(const S of o){const M=u*.4,E=Math.sin(S.angle)*M,y=-Math.cos(S.angle)*M,x=[new W(0,0,1.1),new W(E,y,1.1)],b=new un().setFromPoints(x);t.add(new ao(b,new Pr({color:3355443})))}for(const S of o){const M=u*.35,E=document.createElement("canvas");E.width=64,E.height=64;const y=E.getContext("2d");y.fillStyle="#000",y.font="bold 32px Arial",y.textAlign="center",y.textBaseline="middle",y.fillText(String(S.hour),32,32);const x=new lo(E);x.minFilter=mn;const b=new pa(new oo({map:x}));b.position.set(Math.sin(S.angle)*M,-Math.cos(S.angle)*M,1.5),b.scale.set(6,6,1),t.add(b)}}if(e==="polar"){const{plateSize:i,lines:o,tiltAngle:a,gnomonHeight:u}=s,[c,h]=i,d=new On(c,u*4,h),m=new Cn({color:13944233,roughness:.5,metalness:.1}),_=new zt(d,m);_.rotation.z=a,_.castShadow=!0,_.receiveShadow=!0,t.add(_);const g=new On(c,3,2),S=new zt(g,new Cn({color:8947848,roughness:.3,metalness:.7}));S.position.y=u*2,S.rotation.z=a,t.add(S);for(const M of o){const E=new On(1,1,h+10),y=new zt(E,new Cn({color:3355443}));y.position.set(M.offset,u*2+1,0),y.rotation.z=a,t.add(y);const x=document.createElement("canvas");x.width=64,x.height=64;const b=x.getContext("2d");b.fillStyle="#000",b.font="bold 32px Arial",b.textAlign="center",b.textBaseline="middle",b.fillText(String(M.hour),32,32);const T=new lo(x);T.minFilter=mn;const P=new pa(new oo({map:T}));P.position.set(M.offset,u*2+3,15),P.scale.set(6,6,1),t.add(P)}}return t}function Q_({sceneData:s,type:e}){const t=gt.useRef(null),i=gt.useRef(null),o=gt.useMemo(()=>{if(!s)return null;if(e==="card"){const{width:a,height:u,thickness:c,qrMatrix:h,qrSize:d,qrPosition:m,frontText:_,backText:g,logoPos:S}=s,M=new qi,E=bA(a,u,c);if(M.add(E),h){const y=m||[a*.25,0],x=LA(h,y,d||2,1.5,c/2);M.add(x)}if(_){const y=J0(_,[0,c/2+1,u*.3]);M.add(y)}if(g){const y=J0(g,[0,c/2+1,-u*.3],1,!0);M.add(y)}return M.rotation.x=-Math.PI/2,M}return e==="sundial"?DA(s.geoData,s.sundialType):null},[s,e]);return gt.useEffect(()=>{if(!t.current)return;const a=t.current,u=a.clientWidth,c=a.clientHeight,h=new bT;h.background=new yt(15790320);const d=new ni(45,u/c,1,500);d.position.set(80,60,80),d.lookAt(0,0,0);const m=new PT({antialias:!0});m.setSize(u,c),m.setPixelRatio(window.devicePixelRatio),m.shadowMap.enabled=!0,a.appendChild(m.domElement);const _=new _A(d,m.domElement);_.enableDamping=!0,_.dampingFactor=.1,_.target.set(0,10,0),_.update();const g=new dA(16777215,.6);h.add(g);const S=new Y0(16777215,1);S.position.set(50,100,50),S.castShadow=!0,S.shadow.mapSize.set(1024,1024),h.add(S);const M=new Y0(16777215,.4);M.position.set(-50,30,-50),h.add(M);const E=new pA(100,20,13421772,14540253);h.add(E),o&&h.add(o),i.current={scene:h,camera:d,renderer:m,controls:_};const y=()=>{requestAnimationFrame(y),_.update(),m.render(h,d)};y();const x=()=>{const b=a.clientWidth,T=a.clientHeight;d.aspect=b/T,d.updateProjectionMatrix(),m.setSize(b,T)};return window.addEventListener("resize",x),()=>{window.removeEventListener("resize",x),m.dispose(),_.dispose(),a.contains(m.domElement)&&a.removeChild(m.domElement)}},[o]),fe.jsx("div",{ref:t,style:{width:"100%",height:"100%",minHeight:"400px"}})}const Q0={w:85.6,h:54},gu=1.6;function NA(){const[s,e]=gt.useState("John Doe"),[t,i]=gt.useState("Software Engineer"),[o,a]=gt.useState("Acme Corp"),[u,c]=gt.useState("john@acme.com"),[h,d]=gt.useState("+1 555-0123"),[m,_]=gt.useState("https://acme.com"),[g,S]=gt.useState("https://acme.com"),[M,E]=gt.useState("Scan for more info"),[y,x]=gt.useState(Q0.w),[b,T]=gt.useState(Q0.h),[P,H]=gt.useState(35),[D,N]=gt.useState("right"),[z,C]=gt.useState(null),[w,U]=gt.useState(null),[X,k]=gt.useState(null),[Y,ae]=gt.useState(!1),[te,ce]=gt.useState("front"),[G,he]=gt.useState("M"),re=3,O=y*re,ie=b*re,Ae=Math.min(y,b)*(P/100),Z=gt.useCallback(async()=>{if(g.trim()){ae(!0);try{const ee=await Ug(g.trim(),{errorCorrection:G,size:33,margin:1,width:33});k(ee)}catch(ee){console.error("QR generation failed:",ee)}ae(!1)}},[g,G]);gt.useEffect(()=>{const ee=setTimeout(Z,500);return()=>clearTimeout(ee)},[Z]);const ne=ee=>{const we=ee.target.files[0];if(!we)return;C(we);const Ue=new FileReader;Ue.onload=tt=>U(tt.target.result),Ue.readAsDataURL(we)},xe=async()=>{let ee=X;!ee&&g.trim()&&(ae(!0),ee=await Ug(g.trim(),{errorCorrection:G,size:33,margin:1}),k(ee),ae(!1));const we=[];if(we.push(...yx([0,0,0],[y,b,gu])),ee){const Ue=Ae/ee.length,tt=ee[0].length*Ue,I=ee.length*Ue;let Te,Se;D==="right"?(Te=y-tt-4,Se=(b-I)/2):D==="left"?(Te=4,Se=(b-I)/2):(Te=(y-tt)/2,Se=(b-I)/2);const Oe=Wf(ee,Te,Se,Ue,1.2,gu);if(we.push(...Oe),g.trim()){Wf(ee,Te,Se,Ue,1.2,0);const pe=Wf(ee,Te,Se,Ue,-1.2,gu);we.push(...pe)}}Mx(s.trim()?`${s}_business_card`:"business_card",[we])},de={width:y,height:b,thickness:gu,qrMatrix:X,qrSize:Ae/(IA(X)||1),qrPosition:D==="right"?[y*.6,0]:D==="left"?[-y*.25,0]:[0,0],frontText:s,backText:M,logoPos:D==="right"?[-40,0]:[40,0]};return fe.jsxs("div",{className:"designer-layout",children:[fe.jsxs("div",{className:"designer-panel",children:[fe.jsx("h2",{children:"Business Card Designer"}),fe.jsxs("div",{className:"side-tabs",children:[fe.jsx("button",{className:te==="front"?"active":"",onClick:()=>ce("front"),children:"Front Side"}),fe.jsx("button",{className:te==="back"?"active":"",onClick:()=>ce("back"),children:"Back Side"})]}),fe.jsxs("div",{className:"form-grid",children:[fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"Name"}),fe.jsx("input",{value:s,onChange:ee=>e(ee.target.value),placeholder:"Your Name"})]}),fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"Title"}),fe.jsx("input",{value:t,onChange:ee=>i(ee.target.value),placeholder:"Job Title"})]}),fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"Company"}),fe.jsx("input",{value:o,onChange:ee=>a(ee.target.value),placeholder:"Company"})]}),fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"Email"}),fe.jsx("input",{value:u,onChange:ee=>c(ee.target.value),placeholder:"email@example.com"})]}),fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"Phone"}),fe.jsx("input",{value:h,onChange:ee=>d(ee.target.value),placeholder:"+1 555-0000"})]}),fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"Website"}),fe.jsx("input",{value:m,onChange:ee=>_(ee.target.value),placeholder:"https://..."})]}),fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"QR Code Data"}),fe.jsx("input",{value:g,onChange:ee=>S(ee.target.value),placeholder:"URL or text for QR"})]}),fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"Back Text"}),fe.jsx("input",{value:M,onChange:ee=>E(ee.target.value),placeholder:"Back side text"})]})]}),fe.jsxs("div",{className:"form-row",children:[fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"Card Width (mm)"}),fe.jsx("input",{type:"number",value:y,onChange:ee=>x(Number(ee.target.value)),step:"0.1",min:"50",max:"200"})]}),fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"Card Height (mm)"}),fe.jsx("input",{type:"number",value:b,onChange:ee=>T(Number(ee.target.value)),step:"0.1",min:"30",max:"150"})]})]}),fe.jsxs("div",{className:"form-row",children:[fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"QR Size (%)"}),fe.jsx("input",{type:"range",value:P,onChange:ee=>H(Number(ee.target.value)),min:"15",max:"60"}),fe.jsxs("span",{className:"range-val",children:[P,"%"]})]}),fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"QR Position"}),fe.jsxs("select",{value:D,onChange:ee=>N(ee.target.value),children:[fe.jsx("option",{value:"right",children:"Right"}),fe.jsx("option",{value:"left",children:"Left"}),fe.jsx("option",{value:"center",children:"Center"})]})]})]}),fe.jsxs("div",{className:"form-row",children:[fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"QR Error Correction"}),fe.jsxs("select",{value:G,onChange:ee=>{he(ee.target.value),Z()},children:[fe.jsx("option",{value:"L",children:"Low (7%)"}),fe.jsx("option",{value:"M",children:"Medium (15%)"}),fe.jsx("option",{value:"Q",children:"Quartile (25%)"}),fe.jsx("option",{value:"H",children:"High (30%)"})]})]}),fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"Logo"}),fe.jsx("input",{type:"file",accept:"image/*",onChange:ne}),w&&fe.jsx("img",{src:w,alt:"Logo preview",className:"logo-preview"})]})]}),fe.jsx("button",{className:"btn-primary",onClick:xe,disabled:Y,children:Y?"Generating...":"Export STL for 3D Printing"})]}),fe.jsxs("div",{className:"designer-preview",children:[fe.jsxs("div",{className:"card-preview-container",children:[fe.jsx("h3",{children:"Card Preview"}),fe.jsxs("svg",{width:O,height:ie,viewBox:`0 0 ${y} ${b}`,className:"card-svg",children:[fe.jsx("rect",{width:y,height:b,rx:3,ry:3,fill:"#f5f0e8",stroke:"#ccc",strokeWidth:"0.5"}),te==="front"&&fe.jsxs(fe.Fragment,{children:[w&&fe.jsx("image",{href:w,x:D==="right"?4:D==="left"?8:(y-20)/2,y:4,width:24,height:24,preserveAspectRatio:"xMidYMid meet"}),fe.jsx("text",{x:D==="right"||D==="center"?4:32,y:20,fontSize:"4",fontWeight:"bold",fill:"#222",fontFamily:"Arial",children:s||"Name"}),fe.jsx("text",{x:D==="right"||D==="center"?4:32,y:26,fontSize:"3",fill:"#555",fontFamily:"Arial",children:t||"Title"}),fe.jsx("text",{x:D==="right"||D==="center"?4:32,y:32,fontSize:"2.5",fill:"#777",fontFamily:"Arial",children:o||"Company"}),fe.jsx("text",{x:D==="right"||D==="center"?4:32,y:38,fontSize:"2.5",fill:"#555",fontFamily:"Arial",children:u||"email"}),fe.jsx("text",{x:D==="right"||D==="center"?4:32,y:43,fontSize:"2.5",fill:"#555",fontFamily:"Arial",children:h||"phone"}),fe.jsx("text",{x:D==="right"||D==="center"?4:32,y:48,fontSize:"2.5",fill:"#555",fontFamily:"Arial",children:m||"web"}),X&&fe.jsx("g",{transform:`translate(${D==="right"?y-Ae-3:D==="left"?3:(y-Ae)/2}, ${(b-Ae)/2})`,children:X.map((ee,we)=>ee.map((Ue,tt)=>Ue?fe.jsx("rect",{x:tt*(Ae/ee.length),y:we*(Ae/ee.length),width:Ae/ee.length,height:Ae/ee.length,fill:"#111"},`${we}-${tt}`):null))})]}),te==="back"&&fe.jsxs(fe.Fragment,{children:[fe.jsx("text",{x:y/2,y:b/2,fontSize:"4",fill:"#555",fontFamily:"Arial",textAnchor:"middle",children:M||"Back"}),X&&fe.jsx("g",{transform:`translate(${(y-Ae)/2}, ${(b-Ae)/2})`,children:X.map((ee,we)=>ee.map((Ue,tt)=>Ue?fe.jsx("rect",{x:tt*(Ae/ee.length),y:we*(Ae/ee.length),width:Ae/ee.length,height:Ae/ee.length,fill:"#111"},`b-${we}-${tt}`):null))})]})]})]}),fe.jsx("div",{style:{height:"400px",marginTop:"12px",border:"1px solid #ddd",borderRadius:"8px",overflow:"hidden"},children:fe.jsx(Q_,{sceneData:de,type:"card"})})]})]})}function IA(s){return(s==null?void 0:s.length)||0}const fn=Math.PI/180;function Ta(s){return 23.45*fn*Math.sin(360/365*(284+s)*fn)}function e_(s){const e=.9863013698630136*(s-81)*fn;return 9.87*Math.sin(2*e)-7.53*Math.cos(e)-1.5*Math.sin(e)}function UA(s){const e=s*fn,t=[];for(let i=4;i<=20;i++){if(i===12)continue;const o=(i-12)*15*fn,a=Math.atan(Math.tan(o)*Math.sin(e));t.push({hour:i,angle:a})}return t}function FA(s){const e=s*fn,t=[];for(let i=4;i<=20;i++){if(i===12)continue;const o=(i-12)*15*fn,a=Math.atan(Math.tan(o)*Math.cos(e));t.push({hour:i,angle:a})}return t}function OA(){const s=[];for(let e=4;e<=20;e++)e!==12&&s.push({hour:e,angle:(e-12)*15*fn});return s}function BA(s,e=1){const t=s*fn,i=[],o=e,a=e*Math.sin(t);for(let u=4;u<=20;u++){if(u===12)continue;const c=(u-12)*15*fn;i.push({hour:u,x:o*Math.sin(c),y:a*Math.cos(c)})}return i}function zA(s,e=1){const t=s*fn,i=[],o=[{name:"Jan",doy:15},{name:"Feb",doy:46},{name:"Mar",doy:75},{name:"Apr",doy:105},{name:"May",doy:136},{name:"Jun",doy:166},{name:"Jul",doy:197},{name:"Aug",doy:228},{name:"Sep",doy:258},{name:"Oct",doy:289},{name:"Nov",doy:319},{name:"Dec",doy:350}];for(const a of o){const u=Ta(a.doy),c=e*Math.tan(u)*Math.cos(t);i.push({label:a.name,distance:c})}return i}function ev(s){const e=s*fn,t=355,i=172,o=Ta(t),a=Ta(i),u=Math.tan(o)*Math.cos(e),c=Math.tan(a)*Math.cos(e);return{winterTipDistance:u,summerTipDistance:c,gnomonDirection:e>0?"north":"south"}}function t_(s,e){const t=s/2,i=UA(e),o=[[-t,0,-t],[t,0,-t],[t,0,t],[-t,0,t]],a=t*.5,u=t*.7,c=a,h=2,d=u,m=e*fn,_=-Math.cos(m)*d;return{plateVerts:o,hourLines:i,gnomonAngle:m,gnomonHeight:c,gnomonBaseLen:d,gnomonThickness:h,gnomonTopVertex:[0,c,_]}}function kA(s,e){const t=s/2,i=FA(e),o=t*.5,a=e*fn,u=90*fn-a;return{plateVerts:[[-t,-t,0],[t,-t,0],[t,t,0],[-t,t,0]],hourLines:i,gnomonAngle:u,gnomonHeight:o}}function HA(s,e){const t=s/2,i=(90-e)*fn,o=[[-t,-t,0],[t,-t,0],[t,t,0],[-t,t,0]],a=t*.8;return{plateVerts:o,hourLines:OA(),tiltAngle:i,gnomonLength:a,gnomonRadius:1.5,gnomonDirection:[0,1,0]}}function VA(s,e){const i=s/2*.8,o=BA(e,i),a=zA(e,i),u=ev(e);return{markers:o,dateScale:a,scaleX:i,scaleY:i*Math.sin(e*fn),center:[0,0],cues:u}}function GA(s,e){const t=s/2,o=e*fn,a=[],u=s/12;for(let c=4;c<=20;c++){if(c===12)continue;const h=(c-12)*u;a.push({hour:c,offset:h})}return{plateSize:[s,s*.6],lines:a,tiltAngle:o,gnomonHeight:t*.1}}function WA(s,e,t){const i=ev(s);return{latitude:s,longitude:e,timezone:t,isNorthernHemisphere:s>0,gnomonPoints:i.gnomonDirection,recommendedTypes:["horizontal","vertical","equatorial","analemmatic","polar"],summerSolsticeDeclination:Ta(172),winterSolsticeDeclination:Ta(355),equationOfTimeVariation:{min:e_(1),max:e_(180)}}}const n_=[{id:"horizontal",label:"Horizontal",desc:"Flat on ground. Classic garden sundial. Gnomon angled at your latitude."},{id:"vertical",label:"Vertical (South)",desc:"Wall-mounted. Best on south-facing walls. Good for buildings."},{id:"equatorial",label:"Equatorial",desc:"Dial parallel to equator. Easy to read, hour lines evenly spaced."},{id:"analemmatic",label:"Analemmatic",desc:"Human sundial. Stand on the date marker — your shadow tells time."},{id:"polar",label:"Polar",desc:"Dial parallel to polar axis. Hour lines are parallel to each other."}];function i_(){return-new Date().getTimezoneOffset()/60}function XA(){var P;const[s,e]=gt.useState(40.7128),[t,i]=gt.useState(-74.006),[o,a]=gt.useState(i_()),[u,c]=gt.useState("horizontal"),[h,d]=gt.useState(120),[m,_]=gt.useState(null),[g,S]=gt.useState(!1),[M,E]=gt.useState(""),y=gt.useCallback(()=>{if(!navigator.geolocation){E("Geolocation not supported");return}E("Detecting..."),navigator.geolocation.getCurrentPosition(H=>{e(parseFloat(H.coords.latitude.toFixed(4))),i(parseFloat(H.coords.longitude.toFixed(4))),a(i_()),E(`Location set: ${H.coords.latitude.toFixed(2)}, ${H.coords.longitude.toFixed(2)}`),S(!0)},()=>{E("Permission denied. Using defaults.")},{enableHighAccuracy:!1,timeout:1e4})},[]);gt.useEffect(()=>{_(WA(s,t,o))},[s,t,o]),gt.useEffect(()=>{g&&y()},[g]);const x=(()=>{if(!m)return null;switch(u){case"horizontal":return t_(h,s);case"vertical":return kA(h,s);case"equatorial":return HA(h,s);case"analemmatic":return VA(h,s);case"polar":return GA(h,s);default:return null}})(),b={geoData:x,sundialType:u},T=()=>{const H=`${u}_sundial_lat${Math.abs(s).toFixed(0)}${s>=0?"N":"S"}`,D=[],N=h/2,z=4,C=[[-N,0,-N],[N,0,-N],[N,0,N],[-N,0,N],[-N,z,-N],[N,z,-N],[N,z,N],[-N,z,N]],w=(he,re,O,ie)=>{const Ae=(Z,ne,xe)=>{const de=[ne[0]-Z[0],ne[1]-Z[1],ne[2]-Z[2]],ee=[xe[0]-Z[0],xe[1]-Z[1],xe[2]-Z[2]],we=[de[1]*ee[2]-de[2]*ee[1],de[2]*ee[0]-de[0]*ee[2],de[0]*ee[1]-de[1]*ee[0]],Ue=Math.sqrt(we[0]*we[0]+we[1]*we[1]+we[2]*we[2])||1;for(let tt=0;tt<3;tt++)D.push(...U(we[tt]/Ue));for(const tt of[Z,ne,xe])for(let I=0;I<3;I++)D.push(...U(tt[I]));D.push(0,0)};Ae(he,re,O),Ae(he,O,ie)},U=he=>{const re=new ArrayBuffer(4);return new DataView(re).setFloat32(0,he,!0),new Uint8Array(re)};if(w(C[3],C[2],C[1],C[0]),w(C[4],C[5],C[6],C[7]),w(C[0],C[1],C[5],C[4]),w(C[1],C[2],C[6],C[5]),w(C[2],C[3],C[7],C[6]),w(C[3],C[0],C[4],C[7]),u==="horizontal"){const he=t_(h,s),re=he.gnomonThickness,O=he.gnomonHeight,ie=he.gnomonBaseLen,Ae=s*Math.PI/180,Z=-Math.cos(Ae)*ie,ne=[[-re/2,z,0],[re/2,z,0],[-re/2,z,ie],[re/2,z,ie],[0,z+O,Z]];w(ne[0],ne[1],ne[3],ne[2]),w(ne[0],ne[1],ne[4],ne[4]),w(ne[2],ne[4],ne[4],ne[3]),w(ne[0],ne[2],ne[4],ne[4]),w(ne[1],ne[4],ne[4],ne[3])}const X=D.length/50,k=new Uint8Array(84),Y=new TextEncoder().encode(H.slice(0,79));k.set(Y,0);const ae=new Uint8Array(84+D.length);ae.set(k,0),new DataView(ae.buffer).setUint32(80,X,!0),ae.set(D,84);const te=new Blob([ae],{type:"application/sla"}),ce=URL.createObjectURL(te),G=document.createElement("a");G.href=ce,G.download=H.replace(/\s+/g,"_")+".stl",G.click(),URL.revokeObjectURL(ce)};return fe.jsxs("div",{className:"designer-layout",children:[fe.jsxs("div",{className:"designer-panel",children:[fe.jsx("h2",{children:"Sundial Designer"}),fe.jsxs("div",{className:"form-row",children:[fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"Latitude (°)"}),fe.jsx("input",{type:"number",value:s,onChange:H=>e(Number(H.target.value)),step:"0.01",min:"-90",max:"90"})]}),fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"Longitude (°)"}),fe.jsx("input",{type:"number",value:t,onChange:H=>i(Number(H.target.value)),step:"0.01",min:"-180",max:"180"})]}),fe.jsxs("div",{className:"form-group",children:[fe.jsx("label",{children:"Timezone (UTC)"}),fe.jsx("input",{type:"number",value:o,onChange:H=>a(Number(H.target.value)),step:"0.5",min:"-12",max:"14"})]})]}),fe.jsx("button",{className:"btn-secondary",onClick:y,children:M||"Use My Location"}),M&&fe.jsx("p",{className:"status-msg",children:M}),m&&fe.jsxs("div",{className:"config-info",children:[fe.jsxs("p",{children:[fe.jsx("strong",{children:"Hemisphere:"})," ",m.isNorthernHemisphere?"Northern":"Southern"]}),fe.jsxs("p",{children:[fe.jsx("strong",{children:"Gnomon points:"})," ",m.gnomonPoints]}),fe.jsxs("p",{children:[fe.jsx("strong",{children:"Summer solstice declination:"})," ",m.summerSolsticeDeclination.toFixed(2),"°"]}),fe.jsxs("p",{children:[fe.jsx("strong",{children:"Winter solstice declination:"})," ",m.winterSolsticeDeclination.toFixed(2),"°"]})]}),fe.jsx("h3",{children:"Select Sundial Type"}),fe.jsx("div",{className:"type-grid",children:n_.map(H=>fe.jsxs("button",{className:`type-card ${u===H.id?"active":""}`,onClick:()=>c(H.id),children:[fe.jsx("strong",{children:H.label}),fe.jsx("span",{children:H.desc})]},H.id))}),fe.jsxs("div",{className:"form-group",style:{marginTop:"12px"},children:[fe.jsxs("label",{children:["Dial Size (mm) (",u!=="analemmatic"?"side/total":"major axis",")"]}),fe.jsx("input",{type:"number",value:h,onChange:H=>d(Number(H.target.value)),step:"5",min:"50",max:"500"})]}),fe.jsx("button",{className:"btn-primary",onClick:T,children:"Export STL for 3D Printing"})]}),fe.jsxs("div",{className:"designer-preview",children:[fe.jsxs("div",{className:"sundial-info",children:[fe.jsxs("h3",{children:[(P=n_.find(H=>H.id===u))==null?void 0:P.label," Sundial"]}),fe.jsxs("p",{children:["Latitude: ",s,"° | Longitude: ",t,"° | Size: ",h,"mm"]})]}),fe.jsx("div",{style:{height:"500px",border:"1px solid #ddd",borderRadius:"8px",overflow:"hidden"},children:x&&fe.jsx(Q_,{sceneData:b,type:"sundial"})})]})]})}function jA(){const[s,e]=gt.useState("cards");return fe.jsxs("div",{className:"app",children:[fe.jsxs("header",{className:"app-header",children:[fe.jsx("h1",{children:"3D Print Designer"}),fe.jsxs("nav",{className:"tab-nav",children:[fe.jsx("button",{className:`tab-btn ${s==="cards"?"active":""}`,onClick:()=>e("cards"),children:"Business Cards"}),fe.jsx("button",{className:`tab-btn ${s==="sundials"?"active":""}`,onClick:()=>e("sundials"),children:"Sundials"})]})]}),fe.jsxs("main",{className:"app-main",children:[s==="cards"&&fe.jsx(NA,{}),s==="sundials"&&fe.jsx(XA,{})]})]})}Wy.createRoot(document.getElementById("root")).render(fe.jsx(Oy.StrictMode,{children:fe.jsx(jA,{})}));
